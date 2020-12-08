#pragma once

#include "../vendor/noise/fast_noise_lite.hpp"

#include "../Voxel/Voxel.h"

const static int PERLIN_SEED = 813;
const static int SIMPLEX_SEED = 603;

class Chunk
{
private:
	unsigned char* m_Blocks;
	glm::ivec3 m_Position;
	Voxel::Mesh* m_Mesh;
	std::vector<Voxel::Mesh*> objects;
	int m_ChunkSize;
public:
	Chunk(int x, int y, int z, int chunkSize = 32);
	~Chunk();

	inline glm::ivec3 GetPosition() const { return m_Position; }

	void RenderChunk(Voxel::Renderer* renderer);
private:
	void GenerateChunk();
	int GetBlockOffset(int i, int j, int k);
	unsigned char CheckSurroundingBlocks(int i, int j, int k);
	int CheckBlock(int i, int j, int k);
};