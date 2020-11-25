#pragma once

#include <vector>

#include "Quad.h"

class Cube
{
private:
	float m_Size;
	glm::vec3 m_Position;
	glm::vec4 m_Color;
	//Cache quads
public:
	Cube(glm::vec3 position, glm::vec4 color, float size)
		: m_Position(position), m_Color(color), m_Size(size) {}
	~Cube() {}
	
	std::vector<Quad> GetQuads(unsigned int flags) const;
};