#include <core/Game/Chunk.h>
#include <stdexcept>

void Chunk::init(int width, int height, int depth)
{
    if (width <= 0 || height <= 0 || depth <= 0) { throw std::invalid_argument("Chunk dimensions must be positive!");
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
                    if (y < height / 1.5f) {
                        blocks[index].setType(STONE);
                    }
                    if (y == height - 1) {
                        blocks[index].setType(GRASS);
                    }
                }
            }
        }
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

    block.setPos(glm::vec3(x, y, z));
    block.setVisibleFaces(top, bottom, left, right, front, back);
    if (!block.bVisible) { return false; }
    return true;
}

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos)
{
    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            //int columnHeight = (int)heightMap;
            //std::cout << columnHeight << std::endl;
            for (int y = 0; y < height; ++y) {
                Block& block = getBlock(x, y, z);
                float distance = glm::distance(block.position, cameraPos);
                if (distance > 40.0f) continue;

                if (!sendBlockProps(block, x, y, z)) {
                    std::cout << "x: " << x <<
                        "\ny: " << y <<
                        "\nz: " << z << std::endl;

                    blocks.erase(blocks.begin(), blocks.begin() + blockIndex(x, y, z));
                    continue;
                }
                block.render(textureAtlas, shader);
            }
        }
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return x + (y * width) + (z * width * height);

}
