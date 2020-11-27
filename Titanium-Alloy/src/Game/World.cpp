#include "World.h"

#include <utility>

const static int CHUNKS = 8;

World::World(Renderer* renderer, Camera* camera)
	: m_Renderer(renderer), m_Camera(camera)
{

}

World::~World()
{
    /*for (Chunk* chunk : m_Chunks)
    {
        delete chunk;
    }*/
    for (auto item : m_Chunks)
    {
        delete item.second;
    }
}

void World::GenerateWorld()
{
    for (int i = 0; i < CHUNKS; i++)
    {
        for (int j = 0; j < CHUNKS; j++)
        {
            glm::ivec2 pos = glm::ivec2(i - CHUNKS / 2, j - CHUNKS / 2);
            Chunk* chunk = new Chunk(pos.x, 0, pos.y, CHUNK_SIZE);
            m_Chunks.insert_or_assign(std::make_pair(pos.x, pos.y), chunk);
        }
    }
}

void World::Update()
{
    glm::vec3 camPos = m_Camera->GetPosition();

    // Update variables (make this threaded)
    RemoveChunks(camPos);
    AddChunks(camPos);

    // Update graphics (make this thread safe)
    /*for (Chunk* chunk : m_Chunks)
    {
        chunk->RenderChunk(m_Renderer);
    }*/
    for (auto item : m_Chunks)
    {
        item.second->RenderChunk(m_Renderer);
    }
}

void World::RemoveChunks(glm::vec3 camPos)
{
    std::vector<std::pair<int, int>> toRemove;
    for (const auto& item : m_Chunks) {
        std::pair<int, int> p = item.first;

        glm::vec2 camXZ(camPos.x, camPos.z);
        glm::vec2 chunkXZ((float)CHUNK_SIZE * p.first, (float)CHUNK_SIZE * p.second);

        if (glm::distance(camXZ, chunkXZ) > 100.0f)
        {
            toRemove.push_back(item.first);
        }
    }

    for (auto p : toRemove)
    {
        Chunk* chunk = m_Chunks.at(p);
        delete chunk;
        m_Chunks.erase(p);
    }
}

void World::AddChunks(glm::vec3 camPos)
{
    for (int i = 0; i < CHUNKS; i++)
    {
        for (int j = 0; j < CHUNKS; j++)
        {
            glm::ivec2 camXZ(camPos.x / CHUNK_SIZE, camPos.z / CHUNK_SIZE);
            glm::ivec2 pos = glm::ivec2(i - CHUNKS / 2, j - CHUNKS / 2);
            pos += camXZ;
            if (m_Chunks.count(std::make_pair(pos.x, pos.y)) == 0)
            {
                Chunk* chunk = new Chunk(pos.x, 0, pos.y, CHUNK_SIZE);
                m_Chunks.insert_or_assign(std::make_pair(pos.x, pos.y), chunk);
            }
        }
    }
}
