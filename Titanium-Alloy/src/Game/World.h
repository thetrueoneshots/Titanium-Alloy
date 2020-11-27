#pragma once

#include <vector>
#include <map>

#include "Chunk.h"
#include "../Renderer.h"

static const int CHUNK_SIZE = 32;

class World
{
private:
	//std::vector<Chunk*> m_Chunks;
	std::map<std::pair<int, int>, Chunk*> m_Chunks;
	Renderer* m_Renderer;
	Camera* m_Camera;
public:
	World(Renderer* renderer, Camera* m_Camera);
	~World();

	void GenerateWorld();
	void Update();
private:
	void RemoveChunks(glm::vec3 camPos);
	void AddChunks(glm::vec3 camPos);
};