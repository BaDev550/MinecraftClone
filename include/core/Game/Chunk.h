#pragma once

#include "Block.h"
#include <vector>
#include <unordered_map>
#include <core/Optimization/FrustumCulling.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CHUNK_SIZE 10
#define MAX_HEIGHT 64

class Chunk
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	bool bVisibleOnScreen = true;

	Chunk(glm::vec3& pos) : position(pos) { }
	Chunk() {};
	void init();

	Block* getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType type);
	void addBlock(int x, int y, int z, BlockType type, const glm::vec3& cameraPos, const glm::vec3& cameraFront);
	void removeBlock(int x, int y, int z);

	void renderChunk(Shader& shader, Texture& textureAtlas, glm::vec3 cameraPos, float renderDistance);
	bool sendBlockProps(Block& block, glm::vec3& position);
	void updateVisibility(FrustumCulling& frustum);
	void generateTrain(int chunkX, int chunkZ);

	std::unordered_map<int, Block> blocks;
private:
	unsigned int instance_buffer;

	int blockIndex(int x, int y, int z);
	bool isFaceVisible(int x, int y, int z);
};
