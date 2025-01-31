#pragma once

#include "Block.h"
#include <vector>
#include <unordered_map>
#include <Noise/SimplexNoise.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Chunk
{
public:
	const int MAX_HEIGHT = 64;
	Chunk(int width, int height, int depth) { init(width, height, depth); }
	Chunk() {};
	void init(int width, int height, int depth);

	Block* getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);
	void addBlock(int x, int y, int z, BlockType type, const glm::vec3& cameraPos, const glm::vec3& cameraFront);
	void removeBlock(int x, int y, int z);

	void renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, float renderDistance);
	bool sendBlockProps(Block& block, glm::vec3& position);
	void generateTrain();

	std::unordered_map<int, Block> blocks;

	double heightMap;
private:
	unsigned int instance_buffer;
	int width, height, depth;

	Shader outline_shader;
	int blockIndex(int x, int y, int z);
	bool isFaceVisible(int x, int y, int z);
};
