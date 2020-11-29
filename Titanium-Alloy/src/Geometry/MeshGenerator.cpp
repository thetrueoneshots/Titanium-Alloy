#include "MeshGenerator.h"

#include "glm/glm.hpp"
#include "ColorTypes.h"

Mesh* MeshGenerator::GenerateMesh(MeshGeneratorType t)
{
	switch (t) {
	case MeshGeneratorType::TREE:
		return GenerateTree();
		break;
	default:
		return nullptr;
	}
}

Mesh* MeshGenerator::GenerateTree(int base, int width, int height)
{
	std::vector<Cube*> cubes;
	
	// Base
	for (int i = -width / 6; i < width / 6; i++)
	{
		for (int j = -width / 6; j < width / 6; j++)
		{
			for (int k = 0; k < base; k++)
			{
				Cube* c = new Cube(glm::vec3(i, k, j), COLOR_DARK_BROWN);
				cubes.push_back(c);
			}
		}
	}

	for (int i = -width / 2; i < width / 2; i++) 
	{
		for (int j = -width / 2; j < width / 2; j++)
		{
			for (int k = base; k < height; k++)
			{
				Cube* c = new Cube(glm::vec3(i, k, j), COLOR_DARK_GREEN);
				cubes.push_back(c);
			}
		}
	}

	return new Mesh(cubes);
}

Mesh* MeshGenerator::GenerateFlower()
{
	return nullptr;
}

Mesh* MeshGenerator::GenerateGrass()
{
	return nullptr;
}
