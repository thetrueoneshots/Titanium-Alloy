#pragma once

#include <vector>

#include "Quad.h"

class Cube
{
private:
	float m_Size;
	glm::vec3 m_Position;
	glm::vec4 m_Color;
	unsigned char m_Flags;
	//Cache quads
public:
	Cube(glm::vec3 position, glm::vec4 color, float size, unsigned char flags)
		: m_Position(position), m_Color(color), m_Size(size), m_Flags(flags) {}
	~Cube() {}
	
	std::vector<Quad> GetQuads() const;
};