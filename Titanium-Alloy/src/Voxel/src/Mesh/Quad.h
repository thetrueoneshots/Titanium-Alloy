#pragma once
// Remove file and move logic to [ Mesh.h ]

#include <cstring>

#include "glm/glm.hpp"

#include "DataTypes.h"

namespace Voxel
{
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