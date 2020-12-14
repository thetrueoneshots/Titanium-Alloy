#include "Chunk.h"

#include <new> 
#include <algorithm>

#include <iostream>

#include "../Geometry/ColorTypes.h"
#include "../Geometry/MeshGenerator.h"

#include "../World/WorldGenerator.h"

Chunk::Chunk(int x, int y, int z, int chunkSize)
	: m_Position(glm::ivec3(x, y, z)), m_Mesh(nullptr),
	m_ChunkSize(chunkSize)
{
	int size = m_ChunkSize * m_ChunkSize * m_ChunkSize;
	m_Blocks = new unsigned char[size];
	if (!m_Blocks) return;
	std::fill_n(m_Blocks, size, 0);
	
	GenerateChunk();
}

Chunk::~Chunk()
{
	if (m_Blocks)
	{
		delete[] m_Blocks;
	}
	if (m_Mesh)
	{
		delete m_Mesh;
	}
}

void Chunk::RenderChunk(Voxel::Renderer* renderer)
{
	m_Objects.Render(renderer);

	if (m_Mesh)
	{
		renderer->DrawChunk(m_Mesh);
		return;
	}

	if (!m_Blocks) return;
	glm::vec3 pos = glm::vec3(m_Position.x * m_ChunkSize, m_Position.y * m_ChunkSize, m_Position.z * m_ChunkSize);
	m_Mesh = new Voxel::Mesh();
	m_Mesh->GetTransForm()->SetTranslation(pos);

	for (unsigned char i = 0; i < m_ChunkSize; i++)
	{
		for (unsigned char j = 0; j < m_ChunkSize; j++)
		{
			for (unsigned char k = 0; k < m_ChunkSize; k++)
			{
				unsigned char flag = CheckSurroundingBlocks(i, j, k);
				if (flag >= 63) continue; // 2 ^ 6 - 1 (all collision flags)

				int offset = GetBlockOffset(i, j, k);
				if (offset == -1) continue;

				glm::vec4 color = WorldGenerator::GetBlockColor((BlockType)m_Blocks[offset]);

				if (color.a > 0.0f)
				{
					m_Mesh->AddCube(glm::vec3(i, j, k), color, flag);
				}
			}
		}
	}
	renderer->DrawChunk(m_Mesh);
}

void Chunk::GenerateChunk()
{
	for (unsigned char i = 0; i < m_ChunkSize; i++)
	{
		for (unsigned char k = 0; k < m_ChunkSize; k++)
		{
			glm::vec3 worldPos;
			worldPos.x = (float)m_Position.x * m_ChunkSize + i;
			worldPos.y = (float)m_Position.y * m_ChunkSize;
			worldPos.z = (float)m_Position.z * m_ChunkSize + k;

			float noise = WorldGenerator::GetHeight(worldPos);
			int height = (int)(noise * (m_ChunkSize - 1));

			MeshGeneratorType obj = WorldGenerator::PlaceObject(worldPos, height);
			if (obj != MeshGeneratorType::TYPE_BEGIN)
			{
				glm::vec3 pos = glm::vec3(worldPos.x, height + 1.0f, worldPos.z);
				Voxel::Transform* transform = new Voxel::Transform();
				transform->SetTranslation(pos);
				m_Objects.Add(obj, transform);
			}

			for (unsigned char j = 0; j < m_ChunkSize; j++)
			{
				int offset = GetBlockOffset(i, j, k);
				if (offset == -1) continue;
				unsigned char type = (unsigned char)WorldGenerator::GetBlockType(glm::vec3(i, j, k), j, height);
				m_Blocks[offset] = type;
			}
		}
	}
}

int Chunk::GetBlockOffset(int i, int j, int k) 
{
	if (i < 0 || j < 0 || k < 0) return -1;
	if (i >= m_ChunkSize || j >= m_ChunkSize || k >= m_ChunkSize) return -1;
	return (i * m_ChunkSize * m_ChunkSize) + (j * m_ChunkSize) + k;
}

unsigned char Chunk::CheckSurroundingBlocks(int i, int j, int k)
{
	unsigned char flag = 0;
	
	flag |= CheckBlock(i + 1, j, k) << 0;
	flag |= CheckBlock(i - 1, j, k) << 1;
	flag |= CheckBlock(i, j + 1, k) << 2;
	flag |= CheckBlock(i, j - 1, k) << 3;
	flag |= CheckBlock(i, j, k + 1) << 4;
	flag |= CheckBlock(i, j, k - 1) << 5;

	return flag;
}

int Chunk::CheckBlock(int i, int j, int k)
{
	int offset = GetBlockOffset(i, j, k);
	if (offset == -1) return 0;

	switch (m_Blocks[offset])
	{
	case 0:
	case 6:
		return 0;
		break;
	default:
		return 1;
		break;
	}
}