#include "WorldGenerator.h"

float clamp(float var, float lower, float upper)
{
	return var < lower ? lower : var > upper ? upper : var;
}

float WorldGenerator::GetHeight(const glm::vec3& pos)
{
	const static float PERLIN_AMPLITUDE = 1.0f;
	const static float PERLIN_FREQUENCY = 1.0f;

	const static float SIMPLEX_AMPLITUDE = 0.1f;
	const static float SIMPLEX_FREQUENCY = 5.0f;

	static FastNoiseLite perlinNoise(HEIGHT_PERLIN_SEED);
	static FastNoiseLite simplexNoise(HEIGHT_SIMPLEX_SEED);

	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	simplexNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);

	float perlin = PERLIN_AMPLITUDE * perlinNoise.GetNoise(
		pos.x * PERLIN_FREQUENCY,
		pos.z * PERLIN_FREQUENCY
	);

	float simplex = SIMPLEX_AMPLITUDE * simplexNoise.GetNoise(
		pos.x * SIMPLEX_FREQUENCY,
		pos.z * SIMPLEX_FREQUENCY
	);

	float height = perlin + simplex;
	return clamp(height, 0.0f, 1.0f);
}

BlockType WorldGenerator::GetBlockType(const glm::vec3& pos, int height, int maxHeight)
{
	const static float PERLIN_AMPLITUDE = 3.0f;
	const static float PERLIN_FREQUENCY = 5.f;

	static FastNoiseLite perlinNoise(BLOCK_TYPE_SEED);
	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

	float perlin = PERLIN_AMPLITUDE * perlinNoise.GetNoise(
		pos.x * PERLIN_FREQUENCY,
		pos.y * PERLIN_FREQUENCY,
		pos.z * PERLIN_FREQUENCY
	);

	int dist = maxHeight - height;
	if (dist < 0) return BlockType::AIR;
	if (maxHeight == 0) return BlockType::WATER; //WATER

	float h = height + (perlin);

	if (h <= 5) return BlockType::DIRT; //DIRT

	if (h > 16) return BlockType::STONE; //STONE

	return BlockType::GRASS;
}

glm::vec4 WorldGenerator::GetBlockColor(BlockType b)
{
	if (b < (BlockType)0 || b >= BlockType::BLOCKTYPE_END)
	{
		return BlockColors[0];
	}
	return BlockColors[(int)b];
}

MeshGeneratorType WorldGenerator::PlaceObject(const glm::vec3& pos, int height)
{
	static FastNoiseLite objectNoise(OBJECT_GENERATION_SEED);
	objectNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

	float treeNoiseRes = objectNoise.GetNoise(
		50.0f * pos.x,
		50.0f * pos.y,
		50.0f * pos.z
	);
	
	float typeNoise = objectNoise.GetNoise(
		-10.0f * pos.x + 400.0f,
		80.0f * pos.y + 39.0f,
		42.0f * pos.z - 100.0f
	);

	unsigned int type = (unsigned int)((treeNoiseRes + 1.0f) * (typeNoise + 1.0f) * 309.221f) % 3;

	if (height < 3)
	{
		return MeshGeneratorType::TYPE_BEGIN;
	}

	if (height >= 15)
	{
		if (treeNoiseRes > 0.45f)
		{
			return MeshGeneratorType::DIAMOND_DEPOSIT;
		}
		return MeshGeneratorType::TYPE_BEGIN;
	}

	if (height > 5 && treeNoiseRes > 0.5f)
	{
		return MeshGeneratorType::TREE;
	}

	if (treeNoiseRes > 0.4f)
	{
		switch (type)
		{
		case 0:
			return MeshGeneratorType::FLOWER1;
			break;
		case 1:
			return MeshGeneratorType::FLOWER2;
			break;
		case 2:
			return MeshGeneratorType::FLOWER3;
			break;
		default:
			return MeshGeneratorType::FLOWER1;
			break;
		}
	}

	if (treeNoiseRes > 0.f)
	{
		switch (type)
		{
		case 0:
			return MeshGeneratorType::GRASS1;
			break;
		case 1:
			return MeshGeneratorType::GRASS2;
			break;
		case 2:
			return MeshGeneratorType::GRASS3;
			break;
		default:
			return MeshGeneratorType::GRASS1;
			break;
		}
	}

	return MeshGeneratorType::TYPE_BEGIN;
}