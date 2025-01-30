#pragma once

#include "Block.h"
#include <vector>

class Chunk
{
public:
	const int MAX_HEIGHT = 256;
	Chunk(){}
	void init(int width, int height, int depth);

	Block& getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);
	
	void renderChunk(Shader& shader, Texture& textureAtlas, const glm::mat4& projection, const glm::mat4& view);
	bool sendBlockProps(Block& block, int x, int y, int z);

	std::vector<Block> blocks;
private:
	int width, height, depth;

	int blockIndex(int x, int y, int z);
	bool isFaceVisible(int x, int y, int z);
};
