#include <core/Game/Chunk.h>
#include <stdexcept>

Chunk::Chunk(int width, int height, int depth)
	: width(width), height(height), depth(depth), blocks(height > MAX_HEIGHT ? MAX_HEIGHT : height * width * depth, Block(AIR)) {
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

void Chunk::renderChunk()
{
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Block& block = getBlock(x, y, z);

                if (block.getBlockType() == BlockType::AIR) continue;


            }
        }
    }
}

int Chunk::blockIndex(int x, int y, int z)
{
    return x + width * (z + depth * y);
}
