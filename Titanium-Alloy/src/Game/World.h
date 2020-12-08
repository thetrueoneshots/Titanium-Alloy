#pragma once

#include <vector>
#include <map>
#include <future>
#include <mutex>

#include "../Voxel/Voxel.h"

#include "Chunk.h"


static const int CHUNK_SIZE = 32;

class World
{
private:
	//std::vector<Chunk*> m_Chunks;
	std::map<std::pair<int, int>, Chunk*> m_Chunks;
	Voxel::Renderer* m_Renderer;
	Voxel::Camera* m_Camera;
public:
	World(Voxel::Renderer* renderer, Voxel::Camera* m_Camera);
	~World();

	void GenerateWorld();
	void Update();
private:
	void RemoveChunks(glm::vec3 camPos);
	void AddChunks(glm::vec3 camPos);
};