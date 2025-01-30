#include <core/Game/Block.h>
#include <vector>
#include <iostream>
#include <stdexcept>

void Block::initializeBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	updateBuffers();
}

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

void Block::updateBuffers()
{
	glBindVertexArray(VAO);

	for (int face = 0; face < 6; ++face) {
		if (!visibleFaces[face]) continue;

		glm::vec2 texCoords = getUVCoords(face);
		unsigned int startIdx = vertices.size();

		switch (face) {
		case 0: // Top face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f), texCoords }
				});
			break;
		case 1: // Bottom face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x, position.y, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x, position.y, position.z + 1.0f), texCoords }
				});
			break;
		case 2: // Left face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x, position.y, position.z), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x, position.y, position.z + 1.0f), texCoords }
				});
			break;
		case 3: // Right face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x + 1.0f, position.y, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f), texCoords }
				});
			break;
		case 4: // Front face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x, position.y, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f), texCoords }
				});
			break;
		case 5: // Back face
			vertices.insert(vertices.end(), {
				{ glm::vec3(position.x, position.y, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y, position.z), texCoords },
				{ glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z), texCoords },
				{ glm::vec3(position.x, position.y + 1.0f, position.z), texCoords }
				});
			break;
		}

		indices.insert(indices.end(), {
			startIdx, startIdx + 1, startIdx + 2,
			startIdx, startIdx + 2, startIdx + 3
			});
	}

	if (vertices.size() == 0) return;
	if (indices.size() == 0) return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Block::init()
{
	initializeBuffers();
}

void Block::setVisibleFaces(bool top, bool bottom, bool left, bool right, bool front, bool back)
{
	visibleFaces[0] = top;
	visibleFaces[1] = bottom;
	visibleFaces[2] = left;
	visibleFaces[3] = right;
	visibleFaces[4] = front;
	visibleFaces[5] = back;
	updateBuffers();
}

void Block::render(Texture& textureAtlas, Shader& shader, const glm::mat4& projection, const glm::mat4& view)
{
	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	shader.setMat4("model", model);

	textureAtlas.bind(0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool Block::isSolid() const
{
	switch (type) {
	case AIR:
	case WATER:
	case LAVA:
		return false;
	default:
		return true;
	}
}

bool Block::isLiquid() const
{
	switch (type) {
	case WATER:
	case LAVA:
		return true;
	default:
		return false;
	}
}

glm::vec2 Block::getUVCoords(int face) const
{
	// 0 is top face and 1 is the bottom
	switch (type)
	{
	case GRASS:
		//if (face == 0) return glm::vec2(0.5f, 0.5f);
		//if (face == 1) return glm::vec2(0.25f, 0.0f);
		return glm::vec2(0.125f, 0.375f);
	case DIRT:
		return glm::vec2(0.25f, 0.0f);
	case STONE:
		return glm::vec2(0.5f, 0.0f);
	default:
		return glm::vec2(0.0f, 0.0f);
	}
}
