#include <core/Game/Chunk.h>
#include <stdexcept>

void Chunk::init(int width, int height, int depth)
{
    if (width <= 0 || height <= 0 || depth <= 0) {
        throw std::invalid_argument("Chunk dimensions must be positive!");
    }
    else {
        this->width = width;
        this->height = height;
        this->depth = depth;

        generateTrain();
        outline_shader.setupShader("shaders/core_vertex.glsl", "shaders/outline_fragment.glsl");
    }
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
}

void Chunk::removeBlock(int x, int y, int z)
{
    int index = blockIndex(x, y, z);
    blocks.erase(index);
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

    bool top = (y == height - 1 || !blockExists(x, y + 1, z) || blocks[blockIndex(x, y + 1, z)].getBlockType() == AIR);
    bool bottom = (y == 0 || !blockExists(x, y - 1, z) || blocks[blockIndex(x, y - 1, z)].getBlockType() == AIR);
    bool left = (x == 0 || !blockExists(x - 1, y, z) || blocks[blockIndex(x - 1, y, z)].getBlockType() == AIR);
    bool right = (x == width - 1 || !blockExists(x + 1, y, z) || blocks[blockIndex(x + 1, y, z)].getBlockType() == AIR);
    bool front = (z == depth - 1 || !blockExists(x, y, z + 1) || blocks[blockIndex(x, y, z + 1)].getBlockType() == AIR);
    bool back = (z == 0 || !blockExists(x, y, z - 1) || blocks[blockIndex(x, y, z - 1)].getBlockType() == AIR);

    block.setVisibleFaces(top, bottom, left, right, front, back);
    return block.bVisible;
}

void Chunk::generateTrain()
{
    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < height; ++y) {
                BlockType type = (y == 0) ? BEDROCK : (y == height - 1) ? GRASS : ((y > height / 2) ? DIRT : STONE);

                int index = blockIndex(x, y, z);

                setBlock(x, y, z, type);
                blocks[index].init();
            }
        }
    }
    glGenBuffers(1, &instance_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
    glBufferData(GL_ARRAY_BUFFER, (height * width * depth) * sizeof(glm::mat4), &blocks[0].position, GL_STATIC_DRAW);
}

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, float renderDistance)
{
    shader.use();
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << std::endl;
    }
    for (auto& pair : blocks) {
        Block& block = pair.second;
        float distance = glm::distance(block.position, cameraPos);

        if (distance > renderDistance) continue;
        if (!sendBlockProps(block, block.position)) continue;
        if (!block.bVisible || block.indices.empty()) {
            block.indices.clear();
            block.vertices.clear();
            glBindVertexArray(0);
            textureAtlas.unbind();
            continue;
        }

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        textureAtlas.bind(0);
        glBindVertexArray(block.VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(block.indices.size()), GL_UNSIGNED_INT, 0, (height * width * depth));
        block.indices.clear();
        block.vertices.clear();
        glBindVertexArray(0);
        textureAtlas.unbind();
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return (y * depth + z) * width + x;
}