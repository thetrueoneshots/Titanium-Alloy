#include "Chunk.h"

#include <new> 
#include <algorithm>

#include <iostream>

const unsigned char TEST_FREQUENCY = 1;

static const glm::vec4 COLOR_BROWN = glm::vec4(0.54f, 0.27f, 0.07f, 1.0f);
static const glm::vec4 COLOR_DARK_GREEN = glm::vec4(0.12f, 0.51f, 0.28f, 1.0f);
static const glm::vec4 COLOR_GREEN = glm::vec4(0.22f, 0.71f, 0.38f, 1.0f);
static const glm::vec4 COLOR_GRAY = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
static const glm::vec4 COLOR_BLUE = glm::vec4(0.3f, 0.2f, 0.8f, 1.0f);

char GetBlockType(char height, char maxHeight, float simplex);
int GetBlockOffset(int i, int j, int k);
unsigned char CheckSurroundingBlocks(unsigned char* blocks, int i, int j, int k);
int CheckBlock(unsigned char* blocks, int i, int j, int k);

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

void Chunk::RenderChunk(Renderer* renderer)
{
	if (m_Mesh)
	{
		renderer->DrawChunk(*m_Mesh);
		return;
	}
	if (!m_Blocks) return;
	m_Mesh = new Mesh(glm::vec3(m_Position.x * m_ChunkSize, m_Position.y * m_ChunkSize, m_Position.z * m_ChunkSize));

	for (unsigned char i = 0; i < m_ChunkSize; i++)
	{
		for (unsigned char j = 0; j < m_ChunkSize; j++)
		{
			for (unsigned char k = 0; k < m_ChunkSize; k++)
			{
				unsigned char flag = CheckSurroundingBlocks(i, j, k);
				if (flag >= 63) continue; // 2 ^ 6 - 1 (all collision flags)

				glm::vec4 color;
				int offset = GetBlockOffset(i, j, k);
				if (offset == -1) continue;
				switch (m_Blocks[offset])
				{
				case 1:
					color = COLOR_GREEN;
					break;
				case 2:
					color = COLOR_BROWN;
					break;
				case 3:
					color = COLOR_DARK_GREEN;
					break;
				case 4:
					color = COLOR_GRAY;
					break;
				case 5:
					color = COLOR_BLUE;
					break;
				default:
					color = glm::vec4(0.0f);
					break;
				}
				if (color.a > 0.0f)
				{
					m_Mesh->AddCube(glm::vec3(i, j, k), color, 1.0f, flag);
				}
			}
		}
	}
	renderer->DrawChunk(*m_Mesh);
}

void Chunk::GenerateChunk()
{
	const static float PERLIN_AMPLITUDE = 1.0f;
	const static float PERLIN_FREQUENCY = 1.0f;

	const static float SIMPLEX_AMPLITUDE = 0.1f;
	const static float SIMPLEX_FREQUENCY = 5.0f;

	FastNoiseLite perlinNoise(PERLIN_SEED);
	FastNoiseLite simplexNoise(SIMPLEX_SEED);
	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	simplexNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);

	for (unsigned char i = 0; i < m_ChunkSize; i++)
	{
		for (unsigned char k = 0; k < m_ChunkSize; k++)
		{
			glm::vec3 worldPos;
			worldPos.x = (float)m_Position.x * m_ChunkSize + i;
			worldPos.y = (float)m_Position.y * m_ChunkSize;
			worldPos.z = (float)m_Position.z * m_ChunkSize + k;

			float perlin = PERLIN_AMPLITUDE * perlinNoise.GetNoise(
				worldPos.x * PERLIN_FREQUENCY * TEST_FREQUENCY,
				worldPos.z * PERLIN_FREQUENCY * TEST_FREQUENCY
			);

			float simplex = SIMPLEX_AMPLITUDE * simplexNoise.GetNoise(
				worldPos.x * SIMPLEX_FREQUENCY * TEST_FREQUENCY,
				worldPos.z * SIMPLEX_FREQUENCY * TEST_FREQUENCY
			);

			//int height = (int)((perlin + simplex + 1.0f) / 2.0f * CHUNK_SIZE);
			int height = (int)((perlin + simplex) * m_ChunkSize);
			height = height < 0 ? 0 : height >= m_ChunkSize ? m_ChunkSize - 1 : height;

			for (unsigned char j = 0; j < m_ChunkSize; j++)
			{
				int offset = GetBlockOffset(i, j, k);
				if (offset == -1) continue;
				m_Blocks[offset] = GetBlockType(j, height, perlin);
			}
		}
	}
}

char GetBlockType(char height, char maxHeight, float perlin)
{
	int dist = maxHeight - height;
	if (dist < 0) return 0; //AIR
	if (maxHeight == 0) return 5; //WATER

	height *= (perlin + 1.0f) / 2.0f;

	if (height <= 5) return 2; //DIRT

	if (height > 20) return 4; //STONE
	
	if (height > 15) return 3;

	return 1;
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