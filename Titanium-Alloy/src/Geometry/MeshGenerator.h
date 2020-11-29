#pragma once

#include "Mesh.h"

enum class MeshGeneratorType
{
	TYPE_BEGIN,
	TREE,
	FLOWER,
	GRASS,
	TYPE_END,
};

class MeshGenerator
{
public:
	static Mesh* GenerateMesh(MeshGeneratorType t);
private:
	static Mesh* GenerateTree(int base = 5, int width = 6, int height = 13);
	static Mesh* GenerateFlower();
	static Mesh* GenerateGrass();
};