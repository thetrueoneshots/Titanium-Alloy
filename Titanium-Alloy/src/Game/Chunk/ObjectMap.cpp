#include "ObjectMap.h"

ObjectMap::ObjectMap()
{
}

ObjectMap::~ObjectMap()
{
	for (const auto& pair : m_Map)
	{
		auto& vTransforms = pair.second;
		for (auto& transform : *vTransforms)
		{
			delete transform;
		}
		delete vTransforms;
	}
}

void ObjectMap::Render(Voxel::Renderer* renderer)
{
	for (const auto& p : m_Map)
	{
		MeshGeneratorType t = p.first;
		const auto& vTransforms = p.second;
		renderer->BatchVoxelDraw(*vTransforms, MeshGenerator::GenerateMesh(t));
	}
}

void ObjectMap::Add(MeshGeneratorType t, Voxel::Transform* transform)
{
	if (m_Map.count(t) == 0)
	{
		m_Map.insert_or_assign(t, new std::vector<Voxel::Transform*>);
	}
	const auto& pair = m_Map.find(t);
	pair->second->push_back(transform);
}
