#pragma once

#include "../vendor/noise/fast_noise_lite.hpp"
//#include "glm/gtc/noise.hpp" // No seeds?
#include "../Renderer.h"

const static unsigned char CHUNK_SIZE = 32;
const static int PERLIN_SEED = 1337;
const static int SIMPLEX_SEED = 12;

class Chunk
{
private:
	unsigned char* m_Blocks; // [CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE] ;
	glm::ivec3 m_Position;
	Mesh* m_Mesh;
public:
	Chunk(int x, int y, int z);
	~Chunk();

	void RenderChunk(Renderer* renderer);
private:
	void GenerateChunk();
};