#pragma once

#include <vector>

#include "Chunk.h"
#include "../Renderer.h"

class World
{
private:
	std::vector<Chunk*> m_Chunks;
	Renderer* m_Renderer;
public:
	World(Renderer* renderer);
	~World();

	void GenerateWorld();
	void Update();
};