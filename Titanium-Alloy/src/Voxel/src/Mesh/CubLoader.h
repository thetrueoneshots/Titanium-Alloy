#pragma once
// Remove and move logic to [ Mesh.h ]

#include "Mesh.h"
#include <string>
#include <iostream>
#include <fstream>

namespace Voxel
{
	/*
	* Class for loading cub files. For now it only contains one static function.
	* Todo: Also add this function to the mesh, indirectly calling this framework function
	*	and then remove this file include in "Voxel.h"
	*/
	class CubLoader
	{
	public:
		static Voxel::Mesh* LoadMeshFromFile(const std::string& file, const std::string& path = "res/cub/");
	private:
		CubLoader() {}
		~CubLoader() {}
	};
}