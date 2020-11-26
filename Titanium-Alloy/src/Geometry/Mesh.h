#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Cube.h"
#include "Quad.h"

class Mesh
{
private:
	glm::vec3 m_Position;
	// Todo: Add rotation
	//glm::vec3 m_Rotation;
	std::vector<Cube> m_Cubes;
	// Todo: Cache quads
public:
	Mesh() : m_Position(glm::vec3(0.0f, 0.0f, 0.0f)) {}
	Mesh(glm::vec3 pos) : m_Position(pos) {}
	~Mesh() {}

	void AddCube(glm::vec3 position, glm::vec4 color, float size = 1.0f, unsigned char flags = 0);
	void AddCube(float p1, float p2, float p3, glm::vec4 color, float size = 1.0f, unsigned char flags = 0);

	// Todo: Move to Mesh.cpp
	void SetPosition(glm::vec3 pos)
	{
		m_Position = pos;
	}
	// Todo: Implement Mesh::SetRotation || Mesh::RotateX, Mesh::RotateY, Mesh::RotateZ
	// Todo: Implement Mesh::GetModelMatrix

	std::vector<Quad> GetQuads() const;

	glm::mat4 GetModelMatrix() const;

	inline glm::vec3 GetPosition() const { return m_Position; }
};