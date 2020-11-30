#include "MeshGenerator.h"

#include "glm/glm.hpp"
#include "ColorTypes.h"

Mesh* MeshGenerator::GenerateMesh(MeshGeneratorType t)
{
	switch (t) {
	case MeshGeneratorType::TREE:
		return GenerateTree();
		break;
	case MeshGeneratorType::FLOWER:
		return GenerateFlower();
		break;
	case MeshGeneratorType::GRASS:
		return GenerateGrass();
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
	std::vector<Cube*> cubes;
	
	cubes.push_back(new Cube(glm::vec3(0, 0, 0), COLOR_DARK_GREEN));
	cubes.push_back(new Cube(glm::vec3(0, 1, 0), COLOR_DARK_GREEN));
	cubes.push_back(new Cube(glm::vec3(0, 2, 0), COLOR_BLUE));
	cubes.push_back(new Cube(glm::vec3(0, 3, 0), COLOR_BLUE));
	cubes.push_back(new Cube(glm::vec3(1, 2, 0), COLOR_RED));
	cubes.push_back(new Cube(glm::vec3(-1, 2, 0), COLOR_RED));
	cubes.push_back(new Cube(glm::vec3(0, 2, 1), COLOR_RED));
	cubes.push_back(new Cube(glm::vec3(0, 2, -1), COLOR_RED));

	Mesh* m = new Mesh(cubes);
	m->SetScale(0.25f);
	m->Translate(glm::vec3(0.0f, -0.35f, 0.0f));
	return m;
}

Mesh* MeshGenerator::GenerateGrass()
{
	std::vector<Cube*> cubes;
	
	for (int j = -2; j < 2; j++)
	{
		for (int i = 0; i < 5; i++)
			{
				cubes.push_back(new Cube(glm::vec3(i + (j * 3) % 5, i, (j * 3) % 4), COLOR_DARK_GREEN));
				cubes.push_back(new Cube(glm::vec3(1 + i + (j * 3) % 5, i, (j * 3) % 4), COLOR_GREEN));
			}
	}

	Mesh* m = new Mesh(cubes);
	m->SetScale(0.125f);
	m->Translate(glm::vec3(0.0f, -0.45f, 0.0f));
	m->Rotate(glm::vec3(glm::radians(10.0f), 0.0f, 0.0f));
	return m;
}
