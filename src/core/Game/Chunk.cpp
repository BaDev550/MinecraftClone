#include <core/Game/Chunk.h>
#include <stdexcept>
#include <glm/gtc/noise.hpp> 

int height;

void Chunk::init()
{
}

std::vector<glm::mat4> instanceTransforms;
void Chunk::updateInstanceTransforms(const std::unordered_map<int, Block>& blocks) {
    instanceTransforms.clear();

    for (const auto& block : blocks) {
        if (block.second.bVisible) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), block.second.position);
            instanceTransforms.push_back(transform);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
    glBufferData(GL_ARRAY_BUFFER, instanceTransforms.size() * sizeof(glm::mat4), instanceTransforms.data(), GL_STATIC_DRAW);
}

Block* Chunk::getBlock(int x, int y, int z)
{
    int index = blockIndex(x, y, z);

    auto it = blocks.find(index);
    if (it == blocks.end()) return nullptr;

    return &it->second;
}

void Chunk::setBlock(int x, int y, int z, BlockType type)
{
    if (type == AIR) {
        removeBlock(x, y, z);
        return;
    }

    int index = blockIndex(x, y, z);
    blocks[index] = Block(type, glm::vec3(x, y, z));
    blocks[index].init();
    blocks[index].setPos(glm::vec3(x, y, z));
}

void Chunk::addBlock(int x, int y, int z, BlockType type, const glm::vec3& cameraPos, const glm::vec3& cameraFront)
{
    if (y < 0 || y >= MAX_HEIGHT) return;

    int index = blockIndex(x, y, z);
    blocks[index] = Block(type, glm::vec3(x, y, z));
    blocks[index].init();
    if (getBlock(x, y, z)->getBlockType() == AIR) {
        removeBlock(x, y, z);
        return;
    }
    updateInstanceTransforms(blocks);
}

void Chunk::removeBlock(int x, int y, int z)
{
    int index = blockIndex(x, y, z);
    blocks.erase(index);
    updateInstanceTransforms(blocks);
}

bool Chunk::isFaceVisible(int x, int y, int z) {
    try {
        Block* neighbor = getBlock(x, y, z);
        return !neighbor->isSolid();
    }
    catch (const std::out_of_range&) {
        return true;
    }
}

bool Chunk::sendBlockProps(Block& block, glm::vec3& position) {

    if (block.getBlockType() == AIR) {
        block.setVisibleFaces(false, false, false, false, false, false);
        return false;
    }

    int x = (int)position.x;
    int y = (int)position.y;
    int z = (int)position.z;

    auto blockExists = [&](int x, int y, int z) -> bool {
        int index = blockIndex(x, y, z);
        return blocks.find(index) != blocks.end();
    };

    auto isTransparentBlock = [&](int x, int y, int z) -> bool {
        if (!blockExists(x, y, z)) return true; 
        BlockType neighborType = blocks[blockIndex(x, y, z)].getBlockType();
        return neighborType == AIR || neighborType == GLASS; 
    };

    bool top = (y == CHUNK_SIZE - 1 || isTransparentBlock(x, y + 1, z));
    bool bottom = (y == 0 || isTransparentBlock(x, y - 1, z));
    bool left = (x == 0 || isTransparentBlock(x - 1, y, z));
    bool right = (x == CHUNK_SIZE - 1 || isTransparentBlock(x + 1, y, z));
    bool front = (z == CHUNK_SIZE - 1 || isTransparentBlock(x, y, z + 1));
    bool back = (z == 0 || isTransparentBlock(x, y, z - 1));

    block.setVisibleFaces(top, bottom, left, right, front, back);
    return block.bVisible;
}

void Chunk::updateVisibility(FrustumCulling& frustum)
{
    glm::vec3 min = position;
    glm::vec3 max = position + glm::vec3(CHUNK_SIZE, MAX_HEIGHT, CHUNK_SIZE);

    bVisibleOnScreen = frustum.isBoxInFrustum(min, max);
    updateInstanceTransforms(blocks);
}

float getTerrainHeight(int chunkX, int chunkZ, int x, int z, int seed)
{
    float globalX = chunkX * CHUNK_SIZE + x;
    float globalZ = chunkZ * CHUNK_SIZE + z;

    int hashedSeed = seed ^ (chunkX * 73856093) ^ (chunkZ * 19349663);
    float noiseValue = glm::perlin(glm::vec2(globalX * 0.05f + seed, globalZ * 0.05f + seed));

    return noiseValue * 10.0f;
}

void Chunk::generateTrain(int chunkX, int chunkZ, int seed)
{
    blocks.clear();
    glGenBuffers(1, &instance_buffer);

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            height = seed != 1 ? round(getTerrainHeight(chunkX, chunkZ, x, z, seed)) : 0;
            for (int y = 0; y < CHUNK_SIZE - height; ++y) {
                BlockType type = (y == 0) ? BEDROCK : (y == (CHUNK_SIZE - height) - 1) ? GRASS : ((y > (CHUNK_SIZE - height) / 2) ? DIRT : STONE);

                int index = blockIndex(x, y, z);

                setBlock(x, y, z, type);
            }
        }
    }
    updateInstanceTransforms(blocks);
}

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, float renderDistance)
{
    shader.use();

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << std::endl;
    }

    float distance = glm::distance(position, cameraPos);

    if (bVisibleOnScreen && distance < renderDistance) {
        for (auto& pair : blocks) {
            Block& block = pair.second;

            if (block.isBlockingRay(cameraPos, block.position)) continue;
            if (!sendBlockProps(block, block.position)) continue;
            if (block.indices.empty()) continue;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            shader.setMat4("model", model);

            textureAtlas.bind(0);
            glBindVertexArray(block.VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(block.indices.size()), GL_UNSIGNED_INT, 0, (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE));
            block.indices.clear();
            block.vertices.clear();
            glBindVertexArray(0);
            textureAtlas.unbind();
        }
    }
    else {
        for (auto& pair : blocks) {
            Block& block = pair.second;
            block.indices.clear();
            block.vertices.clear();
            glBindVertexArray(0);
        }
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return (y * CHUNK_SIZE + z) * CHUNK_SIZE + x;
}