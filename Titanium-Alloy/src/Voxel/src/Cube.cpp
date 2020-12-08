#include "Cube.h"

glm::vec4 Voxel::Cube::GetColor() const
{
	return m_Color;
}

glm::vec3 Voxel::Cube::GetPosition() const
{
	return m_Position;
}

void Voxel::Cube::SetFlags(unsigned char flags)
{
	m_Flags = flags;
}

std::vector<Voxel::Quad> Voxel::Cube::GetQuads() const
{
	std::vector<Quad> quads;

	glm::vec3 points[] = {
		m_Position + glm::vec3(.0f, .0f, 1.f), //0
		m_Position + glm::vec3(1.f, .0f, 1.f), //1
		m_Position + glm::vec3(1.f, 1.f, 1.f), //2
		m_Position + glm::vec3(.0f, 1.f, 1.f), //3
		m_Position + glm::vec3(.0f, .0f, .0f), //4
		m_Position + glm::vec3(1.f, .0f, .0f), //5
		m_Position + glm::vec3(1.f, 1.f, .0f), //6
		m_Position + glm::vec3(.0f, 1.f, .0f), //7
	};


	static glm::ivec4 indices[6] = {
		glm::vec4(1, 5, 6, 2), //Right
		glm::vec4(0, 4, 7, 3), //Left
		glm::vec4(3, 2, 6, 7), //Top
		glm::vec4(0, 1, 5, 4), //Bottom
		glm::vec4(0, 1, 2, 3), //Front
		glm::vec4(4, 5, 6, 7), //Back
	};

	static glm::vec3 normals[6] = {
		glm::vec3(-1.0f, 0.0f,  0.0f),	//Right
		glm::vec3(1.0f,  0.0f,  0.0f),  //Left
		glm::vec3(0.0f,  1.0f,  0.0f),	//Top
		glm::vec3(0.0f, -1.0f,  0.0f),	//Bottom
		glm::vec3(0.0f,  0.0f,  1.0f),	//Front
		glm::vec3(0.0f,  0.0f, -1.0f),	//Back
	};

	for (int i = 0; i < 6; i++)
	{
		if (((m_Flags >> i) & 1) == 1) continue;
		glm::ivec4 p = indices[i];
		quads.push_back(Quad(
			points[p.x], points[p.y], points[p.z], points[p.w], m_Color, normals[i]
		));
	}

	return quads;
}