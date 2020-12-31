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
	// Todo: Make chars
	struct Color
	{
		float r, g, b, a;

		Color(const glm::vec4& color)
		{
			this->r = color.r;
			this->g = color.g;
			this->b = color.b;
			this->a = color.a;
		}

		Color(float r, float g, float b, float a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color()
		{
			this->r = 0;
			this->g = 0;
			this->b = 0;
			this->a = 0;
		}
	};
	/*
	* A mesh containing all cubes, as well as model state, such as rotation, translation and scale
	*/
	// Todo: Store width, depth and height, as well as Cube[width*depth*height] containing all colors of the cubes [ Cube{ char[4] color } ]
	class Mesh
	{
	private:
		int m_Width, m_Depth, m_Height;
		Color*** m_Cubes;
		Transform* m_Transform;
		bool m_Updated;
		RenderData* m_RenderData;

	public:
		/*
		* Constructors and deconstructor
		*/
		Mesh(const glm::ivec3& size);
		Mesh(unsigned int w, unsigned int h, unsigned int d);
		~Mesh();

		/*
		* Adding cubes
		*/
		void AddCube(const glm::ivec3& position, const glm::vec4& color);
		void AddCube(unsigned int x, unsigned int y, unsigned int z, const glm::vec4& color);

		void SetData(const std::vector<Cube*>& cubes);

		RenderData* GetRenderData();

		inline Transform* GetTransForm() const { return m_Transform; }

	private:
		void Init(unsigned int w, unsigned int h, unsigned int d);
		unsigned char GetConnectedBlockFlags(const glm::vec3& pos);
		int CheckBlock(unsigned int x, unsigned int y, unsigned int z);
	};
}