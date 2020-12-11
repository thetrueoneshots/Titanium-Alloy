#pragma once
#include "Mesh.h"
#include <string>
#include <iostream>
#include <fstream>

namespace Voxel
{
	class CubLoader
	{
	private:

	public:
		static Voxel::Mesh* LoadMeshFromFile(const std::string& file, const std::string& path = "res/cub/");
	};
}