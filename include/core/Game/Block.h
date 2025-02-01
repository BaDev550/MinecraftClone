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
	bool visibleFaces[6];

	void initializeBuffers();
	void updateBuffers();

	BlockType type;

	std::vector<glm::vec2> getUVCoords(int face);
public:
	std::vector<unsigned int> indices;
	std::vector<Vertex> vertices;

	Block(BlockType type = AIR, glm::vec3 position = glm::vec3(0.0f)) : type(type), position(position) {}
	~Block() {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
	void init();
	void setVisibleFaces(bool top, bool bottom, bool left, bool right, bool front, bool back);

	BlockType getBlockType() const { return type; }
	void setType(BlockType newType) { type = newType; }

	void render(Texture& textureAtlas, Shader& shader);
	void setPos(glm::vec3 pos) { position = pos; }
	void setRot(glm::quat rot) { rotation = rot; }
	bool isBlockingRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

	bool isSolid() const;
	bool isLiquid() const;
	GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0f);
	glm::quat rotation;
	std::vector<glm::vec2> texCoords;
	bool bVisible = true;
};

