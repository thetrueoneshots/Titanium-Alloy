#pragma once
// Todo: Move out of framework code.

#include <map>
#include "Mesh.h"

namespace Voxel
{
	class MeshStore
	{
	private:
		std::map<unsigned int, Mesh*> m_Store;
	public:
		MeshStore();
		~MeshStore();

		bool InsertMesh(unsigned int key, Mesh* m);

		Voxel::Mesh* GetMesh(unsigned int key);
	};
}