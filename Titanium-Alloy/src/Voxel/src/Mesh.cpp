#include "Mesh.h"

#include "glm/gtc/matrix_transform.hpp"

std::pair<int, std::pair<int, int>> ConvertPosToKey(glm::vec3 pos);
glm::vec3 ConvertKeyToPos(std::pair<int, std::pair<int, int>> key);

Voxel::Mesh::Mesh()
	: m_Translation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f)),
	m_State(0)
{
	SetState(State::UPDATED);
}

Voxel::Mesh::~Mesh()
{
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

Voxel::Mesh::Mesh(std::vector<Cube*> cubes)
	:m_Translation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f)),
	m_State(0), m_RenderData(nullptr)
{
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

// Todo: Check if cube position already exists and return true/false
void Voxel::Mesh::AddCube(glm::vec3 position, glm::vec4 color, unsigned char flags)
{
	auto key = ConvertPosToKey(position);
	if (m_Cubes.count(key) == 0)
	{
		m_Cubes.insert_or_assign(key, new Cube(position, color, flags));
		SetState(State::UPDATED);
		assert(GetState(State::UPDATED));
	}
}

void Voxel::Mesh::AddCube(float p1, float p2, float p3, glm::vec4 color, unsigned char flags)
{
	AddCube(glm::vec3(p1, p2, p3), color, flags);
}

void Voxel::Mesh::SetTranslation(const glm::vec3& translation)
{
	m_Translation = translation;
}

void Voxel::Mesh::Translate(const glm::vec3& translation)
{
	m_Translation += translation;
}

void Voxel::Mesh::SetScale(float scale)
{
	SetScale(glm::vec3(scale));
}

void Voxel::Mesh::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

void Voxel::Mesh::Rotate(const glm::vec3& rotation)
{
	m_Rotation += rotation;
}

void Voxel::Mesh::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
}

// Todo: Implement
void Voxel::Mesh::UpdateRenderFlags()
{
	for (auto& item : m_Cubes)
	{
		unsigned char flags = GetConnectedBlockFlags(ConvertKeyToPos(item.first));
		item.second->SetFlags(flags);
	}
}

// Todo: Fix caching
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

RenderData* Voxel::Mesh::GetRenderData()
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

glm::mat4 Voxel::Mesh::GetModelMatrix() const
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_Translation);
	glm::mat4 rx = glm::rotate(trans, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(rx, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(ry, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::scale(rz, m_Scale);
}

bool Voxel::Mesh::GetState(State s) const
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	return ((m_State >> (unsigned char)s) & 1) == 1;
}

bool Voxel::Mesh::SetState(State s)
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	m_State |= 1 << (unsigned char)s;
	return true;
}

bool Voxel::Mesh::UnsetState(State s)
{
	if (s <= State::STATE_BEGIN || s >= State::STATE_END)
	{
		return false;
	}
	m_State &= 255 ^ (1 << (unsigned char)s);
	return true;
}

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

int Voxel::Mesh::CheckBlock(int x, int y, int z)
{
	auto key = ConvertPosToKey(glm::vec3(x, y, z));
	if (m_Cubes.count(key) == 0)
	{
		return 0;
	}
	return m_Cubes.at(key)->GetColor().a == 1.0f ? 1 : 0;
}

std::pair<int, std::pair<int, int>> ConvertPosToKey(glm::vec3 pos)
{
	return std::make_pair(pos.x, std::make_pair(pos.y, pos.z));
}

glm::vec3 ConvertKeyToPos(std::pair<int, std::pair<int, int>> key)
{
	return glm::vec3(key.first, key.second.first, key.second.second);
}
