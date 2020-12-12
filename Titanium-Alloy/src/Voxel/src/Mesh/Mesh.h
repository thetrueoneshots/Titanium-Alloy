#pragma once

// Todo: Check all needed includes
#include <vector>
#include <map>
#include <utility>

#include "glm/glm.hpp"

#include "Cube.h"
#include "Quad.h"

/*
* Struct for returning all the calculated (OpenGL) render data
*/
struct RenderData
{
	float* vertices;
	size_t vertex_array_size;
	unsigned int* indices;
	size_t indices_array_count;
	RenderData(float* v, size_t s1, unsigned int* i, size_t s2)
	{
		vertices = v;
		vertex_array_size = s1;
		indices = i;
		indices_array_count = s2;
	}
};

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

		glm::vec3 m_Translation, m_Scale, m_Rotation;
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
		* Setters
		*/
		// Todo: inline
		void SetTranslation(const glm::vec3& translation);
		// Todo: inline
		void Translate(const glm::vec3& translation);

		// Todo: inline
		void SetScale(float scale);
		// Todo: inline
		void SetScale(const glm::vec3& scale);

		// Todo: inline
		void SetRotation(const glm::vec3& rotation);			// Rotation is radians!
		// Todo: inline
		void Rotate(const glm::vec3& rotation);					// Rotation is radians!

		/*
		* Getters
		*/
		inline glm::vec3 GetTranslation() const { return m_Translation; }
		// Todo: Write width, depth, height getters (inline)
		// Todo: Write scale and rotation getters (inline)

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
		glm::mat4 GetModelMatrix() const;

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