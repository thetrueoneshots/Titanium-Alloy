#include "Mesh.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

Voxel::Mesh::Mesh(const glm::ivec3& size)
{
	Init(size.x, size.y, size.z);
}

Voxel::Mesh::Mesh(unsigned int w, unsigned int h, unsigned int d)
{
	Init(w, h, d);
}

Voxel::Mesh::~Mesh()
{
	delete m_Transform;

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			delete m_Cubes[i][j];
		}
		delete m_Cubes[i];
	}
	delete m_Cubes;


	if (m_RenderData)
	{
		free(m_RenderData->indices);
		free(m_RenderData->vertices);
		delete m_RenderData;
	}
}

void Voxel::Mesh::SetData(const std::vector<Cube*>& cubes)
{
	for (Cube* cube : cubes)
	{
		const glm::ivec3& pos = cube->GetPosition(); 
		if (pos.x < 0 || pos.x >= m_Width || pos.y < 0 || pos.y >= m_Height|| pos.z < 0 || pos.z >= m_Depth)
		{
			std::cout << "Out of bounds" << std::endl;
			continue;
		}
		const glm::vec4& color = cube->GetColor();
		m_Cubes[pos.x][pos.y][pos.z] = Color(color);
	}
}

void Voxel::Mesh::ScaleToSize(float scale, bool keepRatio)
{
	ScaleToSize(glm::ivec3(scale), keepRatio);
}

void Voxel::Mesh::ScaleToSize(const glm::vec3& scale, bool keepRatio)
{
	if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
	{
		return;
	}

	glm::vec3 calcScale;
	if (keepRatio)
	{
		unsigned int max = (m_Width > m_Depth) ? m_Width : m_Depth;
		calcScale = glm::vec3(scale.x / max, scale.y / max, scale.z / max);
	}
	else
	{
		calcScale = glm::vec3(scale.x / m_Width, scale.y / m_Height, scale.z / m_Depth);
	}
	m_Transform->SetScale(calcScale);
}

void Voxel::Mesh::ScaleXToSize(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.x *= scale / m_Width;
	m_Transform->SetScale(current);
}

void Voxel::Mesh::ScaleYToSize(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.y *= scale / m_Height;
	m_Transform->SetScale(current);
}

void Voxel::Mesh::ScaleZToSize(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.z *= scale / m_Depth;
	m_Transform->SetScale(current);
}

void Voxel::Mesh::AddCube(const glm::ivec3& position, const glm::vec4& color)
{
	AddCube(position.x, position.y, position.z, color);
}

// Todo: Resize when a cube out of bounds is added
void Voxel::Mesh::AddCube(unsigned int x, unsigned int y, unsigned int z, const glm::vec4& color)
{
	if (x >= m_Width || y >= m_Height || z >= m_Depth)
	{
		std::cout << "Out of bounds" << std::endl;
		return;
	}

	m_Cubes[x][y][z] = Color(color);
}


// Todo: Create render data without the usage of cubes
Voxel::RenderData* Voxel::Mesh::GetRenderData()
{
	if (m_RenderData != nullptr)
	{
		return m_RenderData;
	}

	std::vector<Quad> quads;

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			for (int k = 0; k < m_Depth; k++)
			{
				Color color = m_Cubes[i][j][k];
				if (color.a == 0.0f)
				{
					continue;
				}

				glm::ivec3 cubePos = glm::ivec3(i, j, k);
				unsigned char flags = GetConnectedBlockFlags(cubePos);

				if (flags == 63)
				{
					continue;
				}

				glm::vec4 cubeCol = glm::vec4(color.r, color.g, color.b, color.a);
				
				Cube c = Cube(cubePos, cubeCol, flags);

				auto cubeQuads = c.GetQuads();
				quads.insert(quads.end(), cubeQuads.begin(), cubeQuads.end());
			}
		}
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

void Voxel::Mesh::Init(unsigned int w, unsigned int h, unsigned int d)
{
	m_RenderData = nullptr;
	m_Transform = new Transform();
	m_Updated = true;

	m_Width = w > 0 ? w : 1;
	m_Height = h > 0 ? h : 1;
	m_Depth = d > 0 ? d : 1;
	m_Cubes = new Color * *[m_Width];

	for (int i = 0; i < m_Width; i++)
	{
		m_Cubes[i] = new Color * [m_Height];

		for (int j = 0; j < m_Height; j++)
		{
			m_Cubes[i][j] = new Color[m_Depth];
		}
	}
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

int Voxel::Mesh::CheckBlock(unsigned int x, unsigned int y, unsigned int z)
{
	if (x >= m_Width || y >= m_Height || z >= m_Depth)
	{
		return 0;
	}

	return m_Cubes[x][y][z].a == 1.0f ? 1 : 0;
}