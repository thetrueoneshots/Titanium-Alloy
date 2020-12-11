#pragma once

#include <vector>

#include "Quad.h"

namespace Voxel
{
	class Cube
	{
	private:
		glm::vec3 m_Position;
		glm::vec4 m_Color;
		unsigned char m_Flags;
		//Cache quads
	public:
		Cube(glm::vec3 position, glm::vec4 color, unsigned char flags = 0)
			: m_Position(position), m_Color(color), m_Flags(flags) {}
		~Cube() {}

		glm::vec4 GetColor() const;
		glm::vec3 GetPosition() const;
		void SetFlags(unsigned char flags);
		std::vector<Quad> GetQuads() const;
	};
}