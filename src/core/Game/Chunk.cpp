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
        this->blocks.resize(height > MAX_HEIGHT ? MAX_HEIGHT : height * width * depth, Block(GRASS));
        for (auto& block : blocks) {
            block.init();
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

    if (block.getBlockType() == AIR) return false;
    bool top = (y == height - 1 || getBlock(x, y + 1, z).getBlockType() == AIR);
    bool bottom = (y == 0 || getBlock(x, y - 1, z).getBlockType() == AIR);
    bool left = (x == 0 || getBlock(x - 1, y, z).getBlockType() == AIR);
    bool right = (x == width - 1 || getBlock(x + 1, y, z).getBlockType() == AIR);
    bool front = (z == depth - 1 || getBlock(x, y, z + 1).getBlockType() == AIR);
    bool back = (z == 0 || getBlock(x, y, z - 1).getBlockType() == AIR);

    block.setPos(glm::vec3(x, y, z));
    block.setVisibleFaces(top, bottom, left, right, front, back);
    return true;
}

void Chunk::renderChunk(Shader& shader, Texture& textureAtlas, const glm::mat4& projection, const glm::mat4& view)
{
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Block& block = getBlock(x, y, z);
                if (!sendBlockProps(block, x, y, z)) continue;
                block.render(textureAtlas, shader, projection, view);
            }
        }
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return x + width * (z + depth * y);
}
