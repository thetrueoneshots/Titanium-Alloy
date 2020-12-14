#pragma once

// Todo: Check all needed includes
#include <vector>
#include <map>
#include <utility>

#include "glm/glm.hpp"

#include "DataTypes.h"
#include "Cube.h"
#include "Quad.h"

#include "../Renderer/Transform.h"

namespace Voxel
{
	/*
	* A mesh containing all cubes, as well as model state, such as rotation, translation and scale
	*/
	// Todo: Store width, depth and height, as well as Cube[width*depth*height] containing all colors of the cubes [ Cube{ char[4] color } ]
	class Mesh
	{
	private:
		// Todo: Remove state, add updated variable [ boolean ]
		static enum class State
		{
			STATE_BEGIN,
			UPDATED = 0,
			STATE_END
		};

		Transform* m_Transform;
		// Todo: Change to a Cube[width * depth * height]
		std::map<std::pair<int, std::pair<int, int>>, Cube*> m_Cubes;
		// Todo: Remove and add updated variable [ boolean ]
		unsigned char m_State;
		// Todo: Remove
		std::vector<Quad> m_Cache;
		// Todo: Decide whether to name this cache or renderdata
		RenderData* m_RenderData;
	public:
		/*
		* Constructors and deconstructor
		*/
		Mesh();
		Mesh(std::vector<Cube*> cubes);
		~Mesh();

		/*
		* Adding cubes
		*/
		// Todo: Rewrite based on new member variables
		void AddCube(glm::vec3 position, glm::vec4 color, unsigned char flags = 0);
		void AddCube(float p1, float p2, float p3, glm::vec4 color, unsigned char flags = 0);

		// Todo: Remove
		std::vector<Quad> GetQuads();
		RenderData* GetRenderData();

		// Todo: Remove and calculate in [ GetRenderData() ]
		void UpdateRenderFlags();

		// Todo: Rename to [ ModelMatrix() | CalculateModelMatrix() ]
		inline Transform* GetTransForm() const { return m_Transform; }

	private:
		// Todo: Rewrite based on new member variables
		unsigned char GetConnectedBlockFlags(const glm::vec3& pos);
		int CheckBlock(int x, int y, int z);

		// Todo: Remove state private functions
		bool GetState(State s) const;
		bool SetState(State s);
		bool UnsetState(State s);
	};
}