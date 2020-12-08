#pragma once

#include <cstring>

#include "glm/glm.hpp"

namespace Voxel
{
	struct Vertex
	{
		float position[3];
		float color[4];
		float normal[3];

		Vertex() {}

		Vertex(glm::vec3 p, glm::vec4 c, glm::vec3 n)
		{
			position[0] = p.x;
			position[1] = p.y;
			position[2] = p.z;
			color[0] = c.x;
			color[1] = c.y;
			color[2] = c.z;
			color[3] = c.w;
			normal[0] = n.x;
			normal[1] = n.y;
			normal[2] = n.z;
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