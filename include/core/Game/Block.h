#pragma once

#include "Chunk.h"
#include <glm/glm.hpp>

enum BlockType {
    AIR,
    DIRT,
	LEAVES,
	WOOD,
	PLANK,
    GRASS,
    STONE,
    WATER,
	LAVA,
	OBSIDIAN,
	SAND,
	GLASS,
	BRICK,
	COBBLESTONE,
	DIAMOND,
	EMERALD,
	GOLD,
	IRON,
	COAL,
	LAPIS,
	REDSTONE,
	NETHERRACK,
	GLOWSTONE,
	ENDSTONE,
	BEDROCK,
};

class Block
{
public:
	BlockType type;
	glm::vec3 position;

	Block(BlockType type = AIR, glm::vec3 position = glm::vec3(0.0f)) : type(type), position(position) {}

	BlockType getBlockType() const { return type; }
	void setType(BlockType newType) { type = newType; }

	void render(Texture& textureAtlas, Shader& shader);

	bool isSolid() const;
	bool isLiquid() const;
	bool isFaceVisible(Chunk& chunk, int x, int y, int z);

	glm::vec2 getUVCoords(int face) const;
};

