#include "World.h"

const static int CHUNKS = 2;

World::World(Renderer* renderer)
	: m_Renderer(renderer)
{

}

World::~World()
{
    for (Chunk* chunk : m_Chunks)
    {
        delete chunk;
    }
}

void World::GenerateWorld()
{
    for (int i = 0; i < CHUNKS; i++)
    {
        for (int j = 0; j < CHUNKS; j++)
        {
            m_Chunks.push_back(new Chunk(i - CHUNKS / 2, 0, j - CHUNKS / 2));
        }
    }
}

void World::Update()
{
    // Update variables

    // Update graphics
    for (Chunk* chunk : m_Chunks)
    {
        chunk->RenderChunk(m_Renderer);
    }
}
