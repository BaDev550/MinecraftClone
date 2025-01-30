#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/Texture.h>
#include <renderer/Shader.h>

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

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Block
{
private:
	GLuint VAO, VBO, EBO;

	bool visibleFaces[6];

	void initializeBuffers();
	void updateBuffers();

	BlockType type;
	glm::vec3 position;
	glm::vec2 texCoords;
public:

	Block(BlockType type = AIR, glm::vec3 position = glm::vec3(0.0f)) : type(type), position(position) {}
	void init();
	void setVisibleFaces(bool top, bool bottom, bool left, bool right, bool front, bool back);

	BlockType getBlockType() const { return type; }
	void setType(BlockType newType) { type = newType; }

	void render(Texture& textureAtlas, Shader& shader, const glm::mat4& projection, const glm::mat4& view);
	void setPos(glm::vec3 pos) { position = pos; }

	bool isSolid() const;
	bool isLiquid() const;

	glm::vec2 getUVCoords(int face) const;
};

