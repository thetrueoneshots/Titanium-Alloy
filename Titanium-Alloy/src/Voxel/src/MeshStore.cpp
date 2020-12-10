#include "MeshStore.h"

Voxel::MeshStore::MeshStore()
{
}

Voxel::MeshStore::~MeshStore()
{
	for (const auto& item : m_Store)
	{
		delete item.second;
	}
}

bool Voxel::MeshStore::InsertMesh(unsigned int key, Mesh* m)
{
	if (m == nullptr || m_Store.count(key) != 0)
	{
		return false;
	}

	m_Store.insert_or_assign(key, m);
	return true;
}

Voxel::Mesh* Voxel::MeshStore::GetMesh(unsigned int key)
{
	const auto& item = m_Store.find(key);
	if (item == m_Store.end())
	{
		return nullptr;
	}
	return item->second;
}
