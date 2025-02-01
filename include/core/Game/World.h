#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Chunk.h"

#include <core/Optimization/FrustumCulling.h>

class World
{
public:
	std::vector<std::vector<Chunk>> w_chunks;
	Chunk* current_chunk;

	int w_ChunksX, w_ChunksZ;
	int renderDistance = 35;
	int seed;

	World() {}

	void generateWorld(int chunkX, int chunkZ);
	void generateTrain();
	void updateChunksVisibility(FrustumCulling& frustum);
	Chunk* getCurrentChunk(const glm::vec3& playerPos);
	void renderNearChunks(const glm::vec3& playerPos, Texture& textureAtlas, Shader& shader, int viewDistance);
};

