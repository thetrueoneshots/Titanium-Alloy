#pragma once

#include "../../Voxel/Voxel.h"

enum class MeshGeneratorType
{
	TYPE_BEGIN = 0,
	TREE,
	FLOWER,
	GRASS,
	TYPE_END,
};

class MeshGenerator
{
public:
	static Voxel::Mesh* GenerateMesh(MeshGeneratorType t);
private:
	static Voxel::Mesh* GenerateTree(int base = 5, int width = 6, int height = 13);
	static Voxel::Mesh* GenerateFlower();
	static Voxel::Mesh* GenerateGrass();
};