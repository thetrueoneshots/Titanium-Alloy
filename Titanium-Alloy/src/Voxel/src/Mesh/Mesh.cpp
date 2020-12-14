#include "Mesh.h"

#include "glm/gtc/matrix_transform.hpp"

// Todo: Remove
std::pair<int, std::pair<int, int>> ConvertPosToKey(glm::vec3 pos);
// Todo: Remove
glm::vec3 ConvertKeyToPos(std::pair<int, std::pair<int, int>> key);

// Todo: Rewrite based on new member variables
Voxel::Mesh::Mesh()
	: m_State(0), m_RenderData(nullptr)
{
	m_Transform = new Transform();
	SetState(State::UPDATED);
}

// Todo: Rewrite based on new member variables
Voxel::Mesh::~Mesh()
{
	delete m_Transform;

	for (auto& item : m_Cubes)
	{
		delete item.second;
	}
	if (m_RenderData)
	{
		free(m_RenderData->indices);
		free(m_RenderData->vertices);
		delete m_RenderData;
	}
}

// Todo: Rewrite based on new member variables
Voxel::Mesh::Mesh(std::vector<Cube*> cubes)
	: m_State(0), m_RenderData(nullptr)
{
	m_Transform = new Transform();
	for (Cube* cube : cubes)
	{
		auto key = ConvertPosToKey(cube->GetPosition());
		if (m_Cubes.count(key) == 0)
		{
			m_Cubes.insert_or_assign(key, cube);
		}
		else {
			delete cube;
		}
	}
	UpdateRenderFlags();
}

// Todo: Rewrite based on new member variables
void Voxel::Mesh::AddCube(glm::vec3 position, glm::vec4 color, unsigned char flags)
{
	auto key = ConvertPosToKey(position);
	if (m_Cubes.count(key) == 0)
	{
		m_Cubes.insert_or_assign(key, new Cube(position, color, flags));
		SetState(State::UPDATED);
		//assert(GetState(State::UPDATED));
	}
}

// Todo: Remove flags variable
void Voxel::Mesh::AddCube(float p1, float p2, float p3, glm::vec4 color, unsigned char flags)
{
	AddCube(glm::vec3(p1, p2, p3), color, flags);
}

// Todo: Remove
void Voxel::Mesh::UpdateRenderFlags()
{
	for (auto& item : m_Cubes)
	{
		unsigned char flags = GetConnectedBlockFlags(ConvertKeyToPos(item.first));
		item.second->SetFlags(flags);
	}
}

// Todo: Remove
std::vector<Voxel::Quad> Voxel::Mesh::GetQuads()
{
	std::vector<Quad> quads;

	for (auto& item : m_Cubes)
	{
		auto cubeQuads = item.second->GetQuads();
		quads.insert(quads.end(), cubeQuads.begin(), cubeQuads.end());
	}

	UnsetState(State::UPDATED);
	assert(!GetState(State::UPDATED));
	return quads;
}

// Todo: Rewrite based on member variables
Voxel::RenderData* Voxel::Mesh::GetRenderData()
{
	if (m_RenderData != nullptr)
	{
		return m_RenderData;
	}

	std::vector<Quad> quads;

	for (auto& item : m_Cubes)
	{
		auto cubeQuads = item.second->GetQuads();
		quads.insert(quads.end(), cubeQuads.begin(), cubeQuads.end());
	}

	size_t verticesSize = quads.size() * 4 * sizeof(Vertex);
	size_t indicesSize = quads.size() * 6 * sizeof(unsigned int);
	float* vertices = (float*)malloc(verticesSize);
	unsigned int* indices = (unsigned int*)malloc(indicesSize);

	int vertexCounter = 0;
	int indexCounter = 0;
	for (int i = 0; i < quads.size(); i++)
	{
		auto data = quads.at(i).GetData();
		memcpy(&vertices[vertexCounter * (sizeof(Vertex) / sizeof(float))], data.vertices, 4 * sizeof(Vertex));
		for (int j = 0; j < 6; j++)
		{
			indices[indexCounter + j] = data.indices[j] + vertexCounter;
		}

		vertexCounter += 4;
		indexCounter += 6;
	}

	m_RenderData = new RenderData(vertices, verticesSize, indices, 6 * quads.size());
	return m_RenderData;
}

// Todo: Remove
bool Voxel::Mesh::GetState(State s) const
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	return ((m_State >> (unsigned char)s) & 1) == 1;
}

// Todo: Remove
bool Voxel::Mesh::SetState(State s)
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	m_State |= 1 << (unsigned char)s;
	return true;
}

// Todo: Remove
bool Voxel::Mesh::UnsetState(State s)
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	m_State &= 255 ^ (1 << (unsigned char)s);
	return true;
}

// Todo: Rewrite based on new member variables
unsigned char Voxel::Mesh::GetConnectedBlockFlags(const glm::vec3& pos)
{
	unsigned char flags = 0;

	flags |= CheckBlock(pos.x + 1, pos.y, pos.z) << 0;
	flags |= CheckBlock(pos.x - 1, pos.y, pos.z) << 1;
	flags |= CheckBlock(pos.x, pos.y + 1, pos.z) << 2;
	flags |= CheckBlock(pos.x, pos.y - 1, pos.z) << 3;
	flags |= CheckBlock(pos.x, pos.y, pos.z + 1) << 4;
	flags |= CheckBlock(pos.x, pos.y, pos.z - 1) << 5;

	return flags;
}

// Todo: Rewrite based on new member variables
int Voxel::Mesh::CheckBlock(int x, int y, int z)
{
	auto key = ConvertPosToKey(glm::vec3(x, y, z));
	if (m_Cubes.count(key) == 0)
	{
		return 0;
	}
	return m_Cubes.at(key)->GetColor().a == 1.0f ? 1 : 0;
}

// Todo: Remove
std::pair<int, std::pair<int, int>> ConvertPosToKey(glm::vec3 pos)
{
	return std::make_pair(pos.x, std::make_pair(pos.y, pos.z));
}

// Todo: Remove
glm::vec3 ConvertKeyToPos(std::pair<int, std::pair<int, int>> key)
{
	return glm::vec3(key.first, key.second.first, key.second.second);
}
