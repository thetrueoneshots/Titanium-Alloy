#pragma once

#include "glm/glm.hpp"
#include "../vendor/noise/fast_noise_lite.hpp"
#include "../Geometry/ColorTypes.h"
#include "../Geometry/MeshGenerator.h"

static enum class BlockType {
	AIR = 0,
	GRASS,
	DIRT,
	STONE,
	WATER,
	LEAFS,
	LOG,
	BLOCKTYPE_END,
};

static glm::vec4 BlockColors[] = {
	glm::vec4(0.0f),
	COLOR_GREEN,
	COLOR_BROWN,
	COLOR_GRAY,
	COLOR_WATER,
	COLOR_DARK_GREEN,
	COLOR_DARK_BROWN,
};

class WorldGenerator
{
public:
	
	const static int HEIGHT_PERLIN_SEED = 813;
	const static int HEIGHT_SIMPLEX_SEED = 603;
	const static int BLOCK_TYPE_SEED = 111;

	const static int OBJECT_GENERATION_SEED = 561;

	const static int MAX_HEIGHT = 32;

	static float GetHeight(const glm::vec3& pos);
	static BlockType GetBlockType(const glm::vec3& pos, int height, int maxHeight);

	static glm::vec4 GetBlockColor(BlockType b);

	static MeshGeneratorType PlaceObject(const glm::vec3& pos, int height);
};