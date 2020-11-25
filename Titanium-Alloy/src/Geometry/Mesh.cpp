#include "Mesh.h"

#include "glm/gtc/matrix_transform.hpp"

// Todo: Check if cube position already exists and return true/false
void Mesh::AddCube(glm::vec3 position, glm::vec4 color, float size)
{
	m_Cubes.push_back(Cube(position, color, size));
}

void Mesh::AddCube(float p1, float p2, float p3, glm::vec4 color, float size)
{
	AddCube(glm::vec3(p1, p2, p3), color, size);
}

std::vector<Quad> Mesh::GetQuads() const
{
	std::vector<Quad> quads;
	for (Cube cube : m_Cubes)
	{
		auto cubeQuads = cube.GetQuads(0);
		quads.insert(quads.end(), cubeQuads.begin(), cubeQuads.end());
	}
	return quads;
}

glm::mat4 Mesh::GetModelMatrix() const
{
	return glm::translate(glm::mat4(1.0f), m_Position);
}
