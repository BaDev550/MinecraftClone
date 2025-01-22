#include <core/Game/Block.h>
#include <stdexcept>

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

bool Block::isFaceVisible(Chunk& chunk, int x, int y, int z) {
	try {
		Block& neighbor = chunk.getBlock(x, y, z);
		return !neighbor.isSolid();
	}
	catch (const std::out_of_range&) {
		return true;
	}
}

glm::vec2 Block::getUVCoords(int face) const
{
	// 0 is top face and 1 is the bottom
	switch (type)
	{
	case GRASS:
		if (face == 0) return glm::vec2(0.0f, 0.0f);
		if (face == 1) return glm::vec2(0.25f, 0.0f);
		return glm::vec2(0.5f, 0.0f);
	case DIRT:
		return glm::vec2(0.25f, 0.0f);
	case STONE:
		return glm::vec2(0.5f, 0.0f);
	default:
		return glm::vec2(0.0f, 0.0f);
	}
}
