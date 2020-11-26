#include "Chunk.h"

#include <new> 
#include <algorithm>

#include <iostream>

const unsigned char NOISE_MULTIPLIER = 5;

static const glm::vec4 COLOR_BROWN = glm::vec4(0.54f, 0.27f, 0.07f, 1.0f);
static const glm::vec4 COLOR_DARK_GREEN = glm::vec4(0.12f, 0.51f, 0.28f, 1.0f);
static const glm::vec4 COLOR_GREEN = glm::vec4(0.22f, 0.71f, 0.38f, 1.0f);
static const glm::vec4 COLOR_GRAY = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

char GetBlockType(char height, char maxHeight, float simplex);
int GetBlockOffset(int i, int j, int k);
unsigned char CheckSurroundingBlocks(unsigned char* blocks, int i, int j, int k);
int CheckBlock(unsigned char* blocks, int i, int j, int k);

Chunk::Chunk(int x, int y, int z)
	: m_Position(glm::ivec3(x, y, z)), m_Mesh(nullptr)//, m_Blocks(nullptr)
{
	int size = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
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
	m_Mesh = new Mesh(glm::vec3(m_Position.x * CHUNK_SIZE, m_Position.y * CHUNK_SIZE, m_Position.z * CHUNK_SIZE));

	for (unsigned char i = 0; i < CHUNK_SIZE; i++)
	{
		for (unsigned char j = 0; j < CHUNK_SIZE; j++)
		{
			for (unsigned char k = 0; k < CHUNK_SIZE; k++)
			{
				unsigned char flag = CheckSurroundingBlocks(m_Blocks, i, j, k);
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
	FastNoiseLite perlinNoise(PERLIN_SEED);
	FastNoiseLite simplexNoise(SIMPLEX_SEED);
	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	simplexNoise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_OpenSimplex2);

	for (unsigned char i = 0; i < CHUNK_SIZE; i++)
	{
		for (unsigned char k = 0; k < CHUNK_SIZE; k++)
		{
			glm::vec3 worldPos;
			worldPos.x = (float)m_Position.x * CHUNK_SIZE + i;
			worldPos.y = (float)m_Position.y * CHUNK_SIZE;
			worldPos.z = (float)m_Position.z * CHUNK_SIZE + k;

			float perlin = perlinNoise.GetNoise(
				worldPos.x,
				worldPos.z
			);

			float simplex = simplexNoise.GetNoise(
				worldPos.x * NOISE_MULTIPLIER,
				worldPos.z * NOISE_MULTIPLIER
			);

			int height = (int)((perlin * simplex + 1.0f) / 2.0f * CHUNK_SIZE);

			for (unsigned char j = 0; j < CHUNK_SIZE; j++)
			{
				int offset = GetBlockOffset(i, j, k);
				if (offset == -1) continue;
				m_Blocks[offset] = GetBlockType(j, height, simplex);
			}
		}
	}
}

char GetBlockType(char height, char maxHeight, float simplex)
{
	int dist = maxHeight - height;
	if (dist < 0) return 0; //AIR

	if (height <= 5) return 2; //DIRT

	if (3 * simplex < dist) return 4; //STONE

	if (dist <= 3)
	{	
		return 1; //GRASS
	}
	else {
		return 2; //DIRT
	}
}

int GetBlockOffset(int i, int j, int k) 
{
	if (i < 0 || j < 0 || k < 0) return -1;
	if (i >= CHUNK_SIZE || j >= CHUNK_SIZE || k >= CHUNK_SIZE) return -1;
	return (i * CHUNK_SIZE * CHUNK_SIZE) + (j * CHUNK_SIZE) + k;
}

unsigned char CheckSurroundingBlocks(unsigned char* blocks, int i, int j, int k)
{
	unsigned char flag = 0;
	
	flag |= CheckBlock(blocks, i + 1, j, k) << 0;
	flag |= CheckBlock(blocks, i - 1, j, k) << 1;
	flag |= CheckBlock(blocks, i, j + 1, k) << 2;
	flag |= CheckBlock(blocks, i, j - 1, k) << 3;
	flag |= CheckBlock(blocks, i, j, k + 1) << 4;
	flag |= CheckBlock(blocks, i, j, k - 1) << 5;

	return flag;
}

int CheckBlock(unsigned char* blocks, int i, int j, int k)
{
	int offset = GetBlockOffset(i, j, k);
	if (offset == -1) return 0;

	switch (blocks[offset])
	{
	case 0:
	case 4:
		return 0;
		break;
	default:
		return 1;
		break;
	}
}