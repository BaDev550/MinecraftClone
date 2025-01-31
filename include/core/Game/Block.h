#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <renderer/Texture.h>
#include <renderer/Shader.h>
#include <vector>
#include <iostream>
#include <stdexcept>

enum BlockType {
	AIR,
	DIRT,
	LEAVES,
	OAK_WOOD,
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
	WOOD_DOOR,
};

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Block
{
private:
	GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };

	bool visibleFaces[6];

	void initializeBuffers();
	void updateBuffers();

	BlockType type;

	std::vector<glm::vec2> getUVCoords(int face);
public:
	Block(BlockType type = AIR, glm::vec3 position = glm::vec3(0.0f)) : type(type), position(position) {}
	void init();
	void setVisibleFaces(bool top, bool bottom, bool left, bool right, bool front, bool back);

	BlockType getBlockType() const { return type; }
	void setType(BlockType newType) { type = newType; }

	void render(Texture& textureAtlas, Shader& shader);
	void setPos(glm::vec3 pos) { position = pos; }
	void setRot(glm::quat rot) { rotation = rot; }

	bool isSolid() const;
	bool isLiquid() const;

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0f);
	glm::quat rotation;
	std::vector<glm::vec2> texCoords;
	bool bVisible = true;
};

