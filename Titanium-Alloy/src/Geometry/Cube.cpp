#include "Cube.h"

// Todo: Move to new file
// Todo: Leave out quads if the according flags are set
std::vector<Quad> Cube::GetQuads(unsigned int flags) const
{
	std::vector<Quad> quads;

	glm::vec3 points[] = {
		m_Position + glm::vec3(-m_Size / 2.0f, -m_Size / 2.0f, m_Size / 2.0f), //0
		m_Position + glm::vec3(m_Size / 2.0f, -m_Size / 2.0f, m_Size / 2.0f), //1
		m_Position + glm::vec3(m_Size / 2.0f, m_Size / 2.0f, m_Size / 2.0f), //2
		m_Position + glm::vec3(-m_Size / 2.0f, m_Size / 2.0f, m_Size / 2.0f), //3
		m_Position + glm::vec3(-m_Size / 2.0f, -m_Size / 2.0f, -m_Size / 2.0f), //4
		m_Position + glm::vec3(m_Size / 2.0f, -m_Size / 2.0f, -m_Size / 2.0f), //5
		m_Position + glm::vec3(m_Size / 2.0f, m_Size / 2.0f, -m_Size / 2.0f), //6
		m_Position + glm::vec3(-m_Size / 2.0f, m_Size / 2.0f, -m_Size / 2.0f), //7
	};


	static glm::ivec4 indices[6] = {
		glm::vec4(0, 1, 2, 3), //Front
		glm::vec4(4, 5, 6, 7), //Back
		glm::vec4(1, 5, 6, 2), //Right
		glm::vec4(0, 4, 7, 3), //Left
		glm::vec4(3, 2, 6, 7), //Top
		glm::vec4(0, 1, 5, 4), //Bottom
	};

	static glm::vec3 normals[6] = {
		glm::vec3( 0.0f,  0.0f,  1.0f),
		glm::vec3( 0.0f,  0.0f, -1.0f),
		glm::vec3( 1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3( 0.0f,  1.0f,  0.0f),
		glm::vec3( 0.0f, -1.0f,  0.0f),
	};

	for (int i = 0; i < 6; i++)
	{
		glm::ivec4 p = indices[i];
		quads.push_back(Quad(
			points[p.x], points[p.y], points[p.z], points[p.w], m_Color, normals[i]
		));
	}

	return quads;
}