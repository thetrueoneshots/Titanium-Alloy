#include "Cube.h"

const static glm::ivec4 s_QuadIndices[6] = {
		glm::vec4(1, 5, 6, 2), //Right
		glm::vec4(0, 4, 7, 3), //Left
		glm::vec4(3, 2, 6, 7), //Top
		glm::vec4(0, 1, 5, 4), //Bottom
		glm::vec4(0, 1, 2, 3), //Front
		glm::vec4(4, 5, 6, 7), //Back
};

const static glm::vec3 s_QuadNormals[6] = {
		glm::vec3(-1.0f, 0.0f,  0.0f),	//Right
		glm::vec3(1.0f,  0.0f,  0.0f),  //Left
		glm::vec3(0.0f,  1.0f,  0.0f),	//Top
		glm::vec3(0.0f, -1.0f,  0.0f),	//Bottom
		glm::vec3(0.0f,  0.0f,  1.0f),	//Front
		glm::vec3(0.0f,  0.0f, -1.0f),	//Back
};

/*
* Getter for m_Color
* @return	{glm::vec4}
*/
glm::vec4 Voxel::Cube::GetColor() const
{
	return m_Color;
}

/*
* Getter for m_Position
* @return	{glm::vec3}
*/
glm::vec3 Voxel::Cube::GetPosition() const
{
	return m_Position;
}

/*
* Setter for m_Flags
* @param	{unsigned char}	flags	Variable to set m_Flags to.
* @return	{void}
*/
void Voxel::Cube::SetFlags(unsigned char flags)
{
	m_Flags = flags;
}

/*
* Calculates the required quads to draw for this cube
* @return	{void}
*/
// Todo: Use only the 8 edge points of the cube to avoid duplication (Uses 1/3 of the vertex data!)
std::vector<Voxel::Quad> Voxel::Cube::GetQuads() const
{
	// Todo: Check if cube needs to be rendered at all. [No | Return][Yes | Continue]
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

	for (int i = 0; i < 6; i++)
	{
		if (((m_Flags >> i) & 1) == 1) continue;
		glm::ivec4 p = s_QuadIndices[i];
		quads.push_back(Quad(
			points[p.x], points[p.y], points[p.z], points[p.w], m_Color, s_QuadNormals[i]
		));
	}

	return quads;
}
