#include "MeshGenerator.h"

#include "glm/glm.hpp"

#include "ColorTypes.h"
#include "../Voxel/Voxel.h"

static std::string FileNames[] = {
	"plants/grass.cub",
	"plants/grass2.cub",
	"plants/grass3.cub",
	"plants/cornflower.cub",
	"plants/fire-scrub.cub",
	"plants/flower1.cub",
	"plants/flower2.cub",
	"plants/flower6.cub",
	"trees/fir-tree.cub",
};

static int s_Scales[] = {
	16,
	16,
	16,
	7,
	16,
	10,
	12,
	16,
	4,
};

static std::vector<std::pair<MeshGeneratorType, int>> FileNameLookup = {
	std::make_pair(MeshGeneratorType::GRASS1, 0),
	std::make_pair(MeshGeneratorType::GRASS2, 1),
	std::make_pair(MeshGeneratorType::GRASS3, 2),
	std::make_pair(MeshGeneratorType::PLANT, 3),
	std::make_pair(MeshGeneratorType::PLANT2, 4),
	std::make_pair(MeshGeneratorType::FLOWER1, 5),
	std::make_pair(MeshGeneratorType::FLOWER2, 6),
	std::make_pair(MeshGeneratorType::FLOWER3, 7),
	std::make_pair(MeshGeneratorType::TREE, 8),
};

static Voxel::Mesh* MeshDB[(int)MeshGeneratorType::TYPE_END];

Voxel::Mesh* MeshGenerator::GenerateMesh(MeshGeneratorType t)
{
	Voxel::Mesh* m = MeshDB[(int)t];
	if (m != nullptr)
	{
		return m;
	}

	Voxel::Mesh* mesh = nullptr;

	switch (t) {
	/*case MeshGeneratorType::TREE:
		mesh = GenerateTree();
		break;*/
	case MeshGeneratorType::FLOWER1:
		mesh = GenerateFlower();
		break;
	/*
	case MeshGeneratorType::FLOWER2:
		mesh = GenerateFlower(1);
		break;
	case MeshGeneratorType::FLOWER3:
		mesh = GenerateFlower(2);
		break;*/
	//case MeshGeneratorType::GRASS1:
	//	mesh = GenerateGrass();
	//	break;
	/*case MeshGeneratorType::GRASS2:
		mesh = GenerateGrass(1);
		break;*/
	/*case MeshGeneratorType::GRASS3:
		mesh = GenerateGrass(2);
		break;*/
	default:
		for (const auto& item : FileNameLookup)
		{
			if (item.first == t)
			{
				std::string file = FileNames[item.second];
				int scale = s_Scales[item.second];
				mesh = Voxel::CubLoader::LoadMeshFromFile(file);
				mesh->GetTransForm()->SetScale(1.0f / scale);
				//mesh->GetTransForm()->Translate(glm::vec3(0.5f - scale / 2, 0, 0.5f - scale / 2));
			}
		}
		break;
	}

	MeshDB[(int)t] = mesh;
	return mesh;
}

Voxel::Mesh* MeshGenerator::GenerateTree(int base, int width, int height)
{
	std::vector<Voxel::Cube*> cubes;
	
	// Base
	for (int i = width / 3; i < 2 * width / 3; i++)
	{
		for (int j = width / 3; j < 2 * width / 3; j++)
		{
			for (int k = 0; k < base; k++)
			{
				Voxel::Cube* c = new Voxel::Cube(glm::vec3(i, k, j), COLOR_DARK_BROWN);
				cubes.push_back(c);
			}
		}
	}

	for (int i = 0; i < width; i++) 
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = base; k < height; k++)
			{
				Voxel::Cube* c = new Voxel::Cube(glm::vec3(i, k, j), COLOR_DARK_GREEN);
				cubes.push_back(c);
			}
		}
	}

	Voxel::Mesh* m = new Voxel::Mesh(width, height, width);
	m->SetData(cubes);

	for (const auto& cube : cubes)
	{
		delete cube;
	}
	return m;
}

Voxel::Mesh* MeshGenerator::GenerateFlower(unsigned int variation)
{
	static float scale = 0.25f;

	glm::vec4 centerColor;
	glm::vec4 leafColor;
	switch (variation)
	{
	case 1:
		centerColor = COLOR_WHITE;
		leafColor = COLOR_BLUE;
		break;
	case 2:
		centerColor = COLOR_WHITE;
		leafColor = COLOR_YELLOW;
		break;
	default:
		centerColor = COLOR_BLUE;
		leafColor = COLOR_RED;
		break;
	}

	std::vector<Voxel::Cube*> cubes;
	
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 0, 1), COLOR_DARK_GREEN));
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 1, 1), COLOR_DARK_GREEN));
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 2, 1), centerColor));
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 3, 1), centerColor));
	cubes.push_back(new Voxel::Cube(glm::vec3(2, 2, 1), leafColor));
	cubes.push_back(new Voxel::Cube(glm::vec3(0, 2, 1), leafColor));
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 2, 2), leafColor));
	cubes.push_back(new Voxel::Cube(glm::vec3(1, 2, 0), leafColor));

	Voxel::Mesh* m = new Voxel::Mesh(1/scale, 1/scale, 1/scale);
	m->SetData(cubes);

	for (const auto& cube : cubes)
	{
		delete cube;
	}

	m->GetTransForm()->SetScale(scale);
	return m;
}

Voxel::Mesh* MeshGenerator::GenerateGrass(unsigned int variation)
{
	static float scale = 0.125f;

	glm::vec3 rotation;

	switch (variation)
	{
	case 1:
		rotation = glm::vec3(10.0f, 0.0f, 0.0f);
		break;
	case 2:
		rotation = glm::vec3(5.0f, 180.0f, 5.0f);
		break;
	default:
		rotation = glm::vec3(-5.0f, -90.0f, -10.0f);
		break;
	}

	std::vector<Voxel::Cube*> cubes;
	
	for (int j = -2; j < 2; j++)
	{
		for (int i = 0; i < 5; i++)
			{
				cubes.push_back(new Voxel::Cube(glm::vec3(i + (j * 3) % 5, i, (j * 3) % 4), COLOR_DARK_GREEN));
				cubes.push_back(new Voxel::Cube(glm::vec3(1 + i + (j * 3) % 5, i, (j * 3) % 4), COLOR_GREEN));
			}
	}

	Voxel::Mesh* m = new Voxel::Mesh(1/scale, 1/scale, 1/scale);
	m->SetData(cubes);
	for (const auto& cube : cubes)
	{
		delete cube;
	}

	m->GetTransForm()->SetScale(scale);
	m->GetTransForm()->Rotate(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)));
	return m;
}
