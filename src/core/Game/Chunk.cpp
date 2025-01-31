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

Block& Chunk::getBlock(int x, int y, int z)
{
    int index = blockIndex(x, y, z);
    if (blocks.find(index) == blocks.end()) {
        throw std::out_of_range("Block coordinates are out of range!");
    }
    return blocks[index];
}

void Chunk::setBlock(int x, int y, int z, BlockType type)
{
    int index = blockIndex(x, y, z);
    blocks[index] = Block(type, glm::vec3(x, y, z));
}

void Chunk::addBlock(int x, int y, int z, BlockType type, const glm::vec3& cameraPos, const glm::vec3& cameraFront)
{
    if (y < 0 || y >= MAX_HEIGHT) return;

    int index = blockIndex(x, y, z);
    blocks[index] = Block(type, glm::vec3(x, y, z));
    blocks[index].init();

    glm::vec3 direction = glm::normalize(cameraFront);
    glm::quat rotation = glm::quat(1, 0, 0, 0);

    if (glm::abs(direction.y) > 0.9f) {
        rotation = glm::quat(1, 0, 0, 0);
    }
    else {
        if (direction.x > 0.5f) {
            rotation = glm::quat(glm::radians(90.0f), glm::vec3(0, 1, 0));
        }
        else if (direction.x < -0.5f) {
            rotation = glm::quat(glm::radians(-90.0f), glm::vec3(0, 1, 0));
        }
        else if (direction.z > 0.5f) {
            rotation = glm::quat(0, 0, 0, 1);
        }
        else {
            rotation = glm::quat(glm::radians(180.0f), glm::vec3(0, 1, 0));
        }
    }

    blocks[index].setRot(rotation);
}

void Chunk::removeBlock(int x, int y, int z)
{
    int index = blockIndex(x, y, z);
    if (blocks.find(index) != blocks.end()) {
        blocks[index].setType(AIR);
    }
}

bool Chunk::isFaceVisible(int x, int y, int z) {
    try {
        Block& neighbor = getBlock(x, y, z);
        return !neighbor.isSolid();
    }
    catch (const std::out_of_range&) {
        return true;
    }
}

bool Chunk::sendBlockProps(Block& block, int x, int y, int z) {

    if (block.getBlockType() == AIR) {
        block.setVisibleFaces(false, false, false, false, false, false);
        return false;
    }

    bool top = (y == height - 1 || getBlock(x, y + 1, z).getBlockType() == AIR);
    bool bottom = (y == 0 || getBlock(x, y - 1, z).getBlockType() == AIR);
    bool left = (x == 0 || getBlock(x - 1, y, z).getBlockType() == AIR);
    bool right = (x == width - 1 || getBlock(x + 1, y, z).getBlockType() == AIR);
    bool front = (z == depth - 1 || getBlock(x, y, z + 1).getBlockType() == AIR);
    bool back = (z == 0 || getBlock(x, y, z - 1).getBlockType() == AIR);

    block.setVisibleFaces(top, bottom, left, right, front, back);
    if (!block.bVisible) { return false; }
    return true;
}

void Chunk::generateTrain()
{
    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            for (int y = 0; y < MAX_HEIGHT; ++y) {
                BlockType type = (y == 0) ? BEDROCK : (y == height - 1) ? GRASS : ((y > height / 2) ? DIRT : STONE);
                int index = blockIndex(x, y, z);
                if (y < height) {
                    setBlock(x, y, z, type);
                    blocks[index].init();
                }
            }
        }
    }
}

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, float renderDistance)
{
    for (auto& pair : blocks) {
        Block& block = pair.second;
        float distance = glm::distance(block.position, cameraPos);

        if (distance > renderDistance)
        {
            continue;
        }
        
        if (!sendBlockProps(block, block.position.x, block.position.y, block.position.z)) {
            removeBlock(block.position.x, block.position.y, block.position.z);
            continue;
        }
        block.render(textureAtlas, shader);
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return x + (y * width) + (z * width * MAX_HEIGHT);
}