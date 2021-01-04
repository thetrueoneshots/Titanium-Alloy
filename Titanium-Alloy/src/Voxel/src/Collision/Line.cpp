#include "Line.h"

#include "Box.h"
#include "../Mesh/Mesh.h"

// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf
static bool DoLineQuery(glm::vec3 position, glm::vec3 normal, glm::vec3 extends)
{
	glm::vec3 DxP = glm::cross(normal, position);

	if (glm::abs(DxP.x) > extends.y * glm::abs(normal.z) + extends.z * glm::abs(normal.y))
	{
		return false;
	}

	if (glm::abs(DxP.y) > extends.x * glm::abs(normal.z) + extends.z * glm::abs(normal.x))
	{
		return false;
	}

	if (glm::abs(DxP.z) > extends.x * glm::abs(normal.y) + extends.y * glm::abs(normal.x))
	{
		return false;
	}

	return true;
}

Voxel::Line::Line(const glm::vec3& position, const glm::vec3& normal)
	: m_Position(position), m_Normal(normal)
{
}

Voxel::Line::Line(const glm::vec3& normal)
	: m_Position(glm::vec3(0)), m_Normal(normal)
{
}

Voxel::Line::~Line()
{
}

// https://www.geometrictools.com/Documentation/IntersectionLineBox.pdf
bool Voxel::Line::CollidesWith(Box b)
{
	glm::vec3 center = (b.GetMax() + b.GetMin()) / 2.0f;
	glm::vec3 extends = (b.GetMax() - b.GetMin()) / 2.0f;

	return DoLineQuery(m_Position - center, m_Normal, extends);
}
