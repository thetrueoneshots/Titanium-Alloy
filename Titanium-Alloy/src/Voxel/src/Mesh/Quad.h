#pragma once
// Remove file and move logic to [ Mesh.h ]

#include <cstring>

#include "glm/glm.hpp"

namespace Voxel
{
	struct Vertex
	{
		int32_t position;
		int32_t color;
		int32_t normal;

		Vertex() : position(0), color(0), normal(0) {}

		Vertex(glm::ivec3 p, glm::vec4 c, glm::vec3 n)
		{
			position = p.x + 127 | p.y + 127 << 8 | p.z + 127 << 16;
			color = (int)(255 * c.x) | (int)(255 * c.y) << 8 | (int)(255 * c.z) << 16 | (int)(255 * c.w) << 24;
			normal = (int)(255 * n.x) | (int)(255 * n.y) << 8 | (int)(255 * n.z) << 16;
		}
	};

	struct QuadData
	{
		Vertex vertices[4];
		unsigned int indices[6];

		QuadData(float v[], unsigned int i[])
		{
			memcpy(vertices, v, 4 * sizeof(Vertex));
			memcpy(indices, i, 6 * sizeof(unsigned int));
		}

		QuadData(Vertex v[], unsigned int i[])
		{
			memcpy(vertices, v, 4 * sizeof(Vertex));
			memcpy(indices, i, 6 * sizeof(unsigned int));
		}
	};

	class Quad
	{
	private:
		glm::vec4 m_Color;
		glm::vec3 m_P1, m_P2, m_P3, m_P4;
		glm::vec3 m_Normal;
	public:
		Quad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec4 color, glm::vec3 normal)
			: m_P1(p1), m_P2(p2), m_P3(p3), m_P4(p4), m_Color(color), m_Normal(normal) {}
		~Quad() {};

		QuadData GetData() const;
	};
}