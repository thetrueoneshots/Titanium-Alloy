#pragma once

#include <map>

#include "../Voxel/Voxel.h"
#include "../Geometry/MeshGenerator.h"

class ObjectMap
{
private:
	std::map<MeshGeneratorType, std::vector<Voxel::Transform*>*> m_Map;
public:
	ObjectMap();
	~ObjectMap();

	void Render(Voxel::Renderer* renderer);
	void Add(MeshGeneratorType t, Voxel::Transform* transform);
};