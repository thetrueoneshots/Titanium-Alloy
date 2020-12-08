#include "World.h"

#include <utility>

const static int CHUNKS = 8;

static std::mutex s_ChunkMutex;
static void AddChunk(std::map<std::pair<int, int>, Chunk*>* map, glm::ivec2 pos);

World::World(Voxel::Renderer* renderer, Voxel::Camera* camera)
	: m_Renderer(renderer), m_Camera(camera)
{

}

World::~World()
{
    for (const auto& item : m_Chunks)
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
    for (const auto& item : m_Chunks)
    {
        item.second->RenderChunk(m_Renderer);
    }
    //std::async(std::launch::async, RenderChunks, &m_Chunks, m_Renderer);
    
}

void World::RemoveChunks(glm::vec3 camPos)
{
    const int HALF_CHUNK_SIZE = CHUNKS * CHUNK_SIZE / 2;
    std::vector<std::pair<int, int>> toRemove;
    for (const auto& item : m_Chunks) {
        std::pair<int, int> p = item.first;

        glm::vec2 camXZ(camPos.x, camPos.z);
        glm::vec2 chunkXZ((float)CHUNK_SIZE * p.first, (float)CHUNK_SIZE * p.second);

        if (glm::max(glm::distance(camXZ.x, chunkXZ.x), glm::distance(camXZ.y, chunkXZ.y)) > HALF_CHUNK_SIZE)
        {
            toRemove.push_back(item.first);
        }
    }

    for (const auto& p : toRemove)
    {
        Chunk* chunk = m_Chunks.at(p);
        delete chunk;
        m_Chunks.erase(p);
    }
}

void World::AddChunks(glm::vec3 camPos)
{
    glm::ivec2 camXZ(camPos.x / CHUNK_SIZE, camPos.z / CHUNK_SIZE);
    for (int i = 0; i < CHUNKS; i++)
    {
        for (int j = 0; j < CHUNKS; j++)
        {
            glm::ivec2 pos = glm::ivec2(i - CHUNKS / 2, j - CHUNKS / 2);
            pos += camXZ;
            std::async(std::launch::async, AddChunk, &m_Chunks, pos);
        }
    }
}

static void AddChunk(std::map<std::pair<int, int>, Chunk*>* map, glm::ivec2 pos)
{
    if (map->count(std::make_pair(pos.x, pos.y)) == 0)
    {
        Chunk* chunk = new Chunk(pos.x, 0, pos.y, CHUNK_SIZE);

        std::lock_guard<std::mutex> lock(s_ChunkMutex);
        map->insert_or_assign(std::make_pair(pos.x, pos.y), chunk);
        std::lock_guard<std::mutex> unlock();
    }
}