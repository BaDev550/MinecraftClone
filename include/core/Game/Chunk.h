#pragma once

#include "Block.h"
#include <vector>
#include <unordered_map>
#include <Noise/SimplexNoise.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Chunk
{
public:
	const int MAX_HEIGHT = 64;
	Chunk(int width, int height, int depth) { init(width, height, depth); }
	void init(int width, int height, int depth);

	Block& getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);
	void addBlock(int x, int y, int z, BlockType type);
	void removeBlock(int x, int y, int z);

	void renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, bool outline);
	bool sendBlockProps(Block& block, int x, int y, int z);
	void generateTrain();

	std::unordered_map<int, Block> blocks;

	double heightMap;
private:
	int width, height, depth;

	Shader outline_shader;
	int blockIndex(int x, int y, int z);
	bool isFaceVisible(int x, int y, int z);
};
