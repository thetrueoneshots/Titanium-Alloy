#pragma once

#include "../../Voxel/Voxel.h"

enum class MeshGeneratorType
{
	TYPE_BEGIN = 0,
	TREE,
	FLOWER1,
	FLOWER2,
	FLOWER3,
	GRASS1,
	GRASS2,
	GRASS3,
	TYPE_END,
};

class MeshGenerator
{
public:
	static Voxel::Mesh* GenerateMesh(MeshGeneratorType t);
private:
	static Voxel::Mesh* GenerateTree(int base = 5, int width = 6, int height = 13);
	static Voxel::Mesh* GenerateFlower(unsigned int variation = 0);
	static Voxel::Mesh* GenerateGrass(unsigned int variation = 0);
};