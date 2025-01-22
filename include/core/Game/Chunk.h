#pragma once

#include "Block.h"
#include <vector>

class Chunk
{
public:
	const int MAX_HEIGHT = 256;
	Chunk(int width, int height, int depth);

	Block& getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);

	void renderChunk();
private:
	int width, height, depth;
	std::vector<Block> blocks;

	int blockIndex(int x, int y, int z);
};
