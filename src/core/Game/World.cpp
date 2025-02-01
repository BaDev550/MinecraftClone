#include <core/Game/World.h>
#include <glm/gtc/noise.hpp> 
#include <future>

void World::generateWorld(int chunkX, int chunkZ)
{
	if (chunkX < 8 || chunkZ < 8) {
		std::cout << "WORLD GEN FAILED:min chunk size is 8" << std::endl;
		return;
	}

	w_ChunksX = chunkX;
	w_ChunksZ = chunkZ;
	generateTrain();

	if (seed == 0) {
		std::srand(std::time(nullptr));
		seed = std::rand();
	}
}

void World::generateTrain()
{
	w_chunks.clear();
	w_chunks.resize(w_ChunksX, std::vector<Chunk>(w_ChunksZ));

	for (int x = 0; x < w_ChunksX; x++) {
		for (int z = 0; z < w_ChunksZ; z++) {
			glm::vec3 pos = glm::vec3(x * CHUNK_SIZE, 0, z * CHUNK_SIZE);

			Chunk newChunk(pos);
			w_chunks[x][z] = newChunk;
			w_chunks[x][z].generateTrain(x, z, seed);
		}
	}
}

void World::updateChunksVisibility(FrustumCulling& frustum)
{
	for (int x = 0; x < w_ChunksX; x++) {
		for (int z = 0; z < w_ChunksZ; z++) {
			w_chunks[x][z].updateVisibility(frustum);
		}
	}
}

Chunk* World::getCurrentChunk(const glm::vec3& playerPos)
{
	if (w_chunks.empty()) return NULL;

	int chunkX = static_cast<int>(std::floor(playerPos.x / CHUNK_SIZE));
	int chunkZ = static_cast<int>(std::floor(playerPos.z / CHUNK_SIZE));

	if (chunkX < 0 || chunkZ < 0) return NULL;
	if (chunkX >= static_cast<int>(w_chunks.size())) return NULL;
	if (w_chunks[chunkX].empty() || chunkZ >= static_cast<int>(w_chunks[chunkX].size())) return NULL;

	return &w_chunks[chunkX][chunkZ];
}


void World::renderNearChunks(const glm::vec3& playerPos, Texture& textureAtlas, Shader& shader, int viewDistance)
{
	int currentChunkX = static_cast<int>(floor(playerPos.x / viewDistance));
	int currentChunkZ = static_cast<int>(floor(playerPos.z / viewDistance));

	for (int x = currentChunkX - 1; x <= currentChunkX + 1; x++) {
		for (int z = currentChunkZ - 1; z <= currentChunkZ + 1; z++) {
			if (x < 0 || x >= w_ChunksX || z < 0 || z >= w_ChunksZ)
				continue;

			w_chunks[x][z].renderChunk(shader, textureAtlas, playerPos, renderDistance);
		}
	}
}
