#include <core/Game/Block.h>

void Block::initializeBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	updateBuffers();
}

void Block::updateBuffers()
{
	glBindVertexArray(VAO);

	for (int face = 0; face < 6; ++face) {
		if (!visibleFaces[face]) continue;

		texCoords = getUVCoords(face);
		unsigned int startIdx = round(vertices.size());

		glm::vec3 positions[4];
		switch (face) {
		case 0: // Top face
			positions[0] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z);
			positions[1] = glm::vec3(position.x, position.y + 1.0f, position.z);
			positions[2] = glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f);
			positions[3] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f);
			break;
		case 1: // Bottom face
			positions[0] = glm::vec3(position.x, position.y, position.z);
			positions[1] = glm::vec3(position.x + 1.0f, position.y, position.z);
			positions[2] = glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f);
			positions[3] = glm::vec3(position.x, position.y, position.z + 1.0f);
			break;
		case 2: // Left face
			positions[0] = glm::vec3(position.x, position.y, position.z);
			positions[1] = glm::vec3(position.x, position.y + 1.0f, position.z);
			positions[2] = glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f);
			positions[3] = glm::vec3(position.x, position.y, position.z + 1.0f);
			break;
		case 3: // Right face
			positions[0] = glm::vec3(position.x + 1.0f, position.y, position.z);
			positions[1] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z);
			positions[2] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f);
			positions[3] = glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f);
			break;
		case 4: // Front face
			positions[0] = glm::vec3(position.x, position.y, position.z + 1.0f);
			positions[3] = glm::vec3(position.x + 1.0f, position.y, position.z + 1.0f);
			positions[2] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z + 1.0f);
			positions[1] = glm::vec3(position.x, position.y + 1.0f, position.z + 1.0f);
			break;
		case 5: // Back face
			positions[0] = glm::vec3(position.x, position.y, position.z);
			positions[3] = glm::vec3(position.x + 1.0f, position.y, position.z);
			positions[2] = glm::vec3(position.x + 1.0f, position.y + 1.0f, position.z);
			positions[1] = glm::vec3(position.x, position.y + 1.0f, position.z);
			break;
		}

        vertices.push_back({ positions[0], texCoords[0] });
        vertices.push_back({ positions[1], texCoords[1] });
        vertices.push_back({ positions[2], texCoords[2] });
        vertices.push_back({ positions[3], texCoords[3] });

        indices.insert(indices.end(), {
            startIdx, startIdx + 1, startIdx + 2,
            startIdx + 2, startIdx + 3, startIdx
        });
	}
	if (vertices.empty()) return;
	if (indices.empty()) return;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Block::init()
{
	for (int face = 0; face < 6; ++face) visibleFaces[face] = true;
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
	if ((top && bottom && left && right && front && back) == true) { bVisible = false; }
	updateBuffers();
}

void Block::render(Texture& textureAtlas, Shader& shader)
{
	// Moved to chunk class for instancing
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

std::vector<glm::vec2> Block::getUVCoords(int face)
{
	// 0 is top face and 1 is the bottom
	switch (type)
	{
	case GRASS:
		if (face == 0) {
			return {
				glm::vec2(0.06250f, 0.93750f),
				glm::vec2(0.06250f, 1.00000f),
				glm::vec2(0.09375f, 1.00000f),
				glm::vec2(0.09375f, 0.93750f),
			};
		}
		if (face == 1) {
			return {
				glm::vec2(0.56250f, 0.87500f),
				glm::vec2(0.56250f, 0.93750f),
				glm::vec2(0.59375f, 0.93750f),
				glm::vec2(0.59375f, 0.87500f),
			};
		}
		return {
			glm::vec2(0.31250f, 0.93750f),
			glm::vec2(0.31250f, 1.00000f),
			glm::vec2(0.34375f, 1.00000f),
			glm::vec2(0.34375f, 0.93750f),
		};
	case DIRT:
		return {
			glm::vec2(0.56250f, 0.87500f),
			glm::vec2(0.56250f, 0.93750f),
			glm::vec2(0.59375f, 0.93750f),
			glm::vec2(0.59375f, 0.87500f),
		};
	case STONE:
		return {
			glm::vec2(0.59375f, 0.93750f),
			glm::vec2(0.59375f, 1.00000f),
			glm::vec2(0.62500f, 1.00000f),
			glm::vec2(0.62500f, 0.93750f),
		};
	case COBBLESTONE:
		return {
			glm::vec2(0.81250f, 0.93750f),
			glm::vec2(0.81250f, 1.00000f),
			glm::vec2(0.84375f, 1.00000f),
			glm::vec2(0.84375f, 0.93750f),
		};
	case PLANK:
		return {
			glm::vec2(0.65625f, 0.87500f),
			glm::vec2(0.65625f, 0.93750f),
			glm::vec2(0.68750f, 0.93750f),
			glm::vec2(0.68750f, 0.87500f),
		};
	case OAK_WOOD:
		if (face == 0 || face == 1) {
			return {
				glm::vec2(0.12500f, 0.75000f),
				glm::vec2(0.12500f, 0.81250f),
				glm::vec2(0.15625f, 0.81250f),
				glm::vec2(0.15625f, 0.75000f),
			};
		}
		return {
			glm::vec2(0.09375f, 0.75000f),
			glm::vec2(0.09375f, 0.81250f),
			glm::vec2(0.12500f, 0.81250f),
			glm::vec2(0.12500f, 0.75000f),
		};
	case BEDROCK:
		return {
			glm::vec2(0.00000f, 0.87500f),
			glm::vec2(0.00000f, 0.93750f),
			glm::vec2(0.03125f, 0.93750f),
			glm::vec2(0.03125f, 0.87500f),
		};
	case WATER:
		return {
			glm::vec2(0.40625f, 0.25000f),
			glm::vec2(0.40625f, 0.31250f),
			glm::vec2(0.43750f, 0.31250f),
			glm::vec2(0.43750f, 0.25000f),
		};
	default:
		return {
			glm::vec2(0.0f, 0.0f),
		};
	}
}
