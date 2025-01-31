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
        this->blocks.resize(height > MAX_HEIGHT ? MAX_HEIGHT : height * width * depth, Block(DIRT));
        for (auto& block : blocks) {
            block.init();
        }

        SimplexNoise noise;
        noise.setSeed(sizeof(double) * 10000 / rand());

        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                //double xPos = double(x) / double(width) - 0.5;
                //double yPos = double(y) / double(height) - 0.5;

                //heightMap = abs(noise.signedFBM(xPos, yPos, 2, 1.5f, 0.4f) * 10);
                //std::cout << heightMap << std::endl;
                for (int z = 0; z < depth; ++z) {
                    int index = blockIndex(x, y, z);
                    blocks[index].setPos(glm::vec3(x, y, z));

                    if (y < height / 1.5f) {
                        blocks[index].setType(STONE);
                    }
                    if (y == height - 1) {
                        blocks[index].setType(GRASS);
                    }

                    for (int maxY = abs(height - MAX_HEIGHT); maxY < MAX_HEIGHT; maxY++) 
                    {
                        //addBlock(x, maxY, z, AIR);
                    }
                }
            }
        }

        outline_shader.setupShader("shaders/core_vertex.glsl", "shaders/outline_fragment.glsl");
    }
}

Block& Chunk::getBlock(int x, int y, int z)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height ||
        z < 0 || z >= depth) {
        throw std::out_of_range("Block coordinates are out of range!");
    }
    return blocks[blockIndex(x, y, z)];
}

void Chunk::setBlock(int x, int y, int z, BlockType type)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height ||
        z < 0 || z >= depth) {
        throw std::out_of_range("Block coordinates are out of range!");
    }
    blocks[blockIndex(x, y, z)].setType(type);
}

void Chunk::addBlock(int x, int y, int z, BlockType type)
{
    blocks.push_back(Block(type, glm::vec3(x, y, z)));
    blocks[blocks.size() - 1].init();
}

void Chunk::removeBlock(int x, int y, int z)
{
    if (x < 0 || x >= width ||
        y < 0 || y >= height ||
        z < 0 || z >= depth) {
        throw std::out_of_range("Block coordinates are out of range!");
    }
    blocks[blockIndex(x, y, z)].setType(AIR);
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

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, bool outline)
{
    for (int worldSize = 0; worldSize < blocks.size(); worldSize++) {
        Block& block = blocks[worldSize];
        float distance = glm::distance(block.position, cameraPos);
        if (distance > 30.0f) continue;
        //{ //removeBlock(block.position.x, block.position.y, block.position.z);  }
        
        if (!sendBlockProps(block, block.position.x, block.position.y, block.position.z)) {
            removeBlock(block.position.x, block.position.y, block.position.z);
            continue;
        }
        if (outline) {
            block.render(textureAtlas, shader);
        }
        else {
            block.render(textureAtlas, shader);
        }
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return x + (y * width) + (z * width * height);
}