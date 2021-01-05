#include "Box.h"

Voxel::Box::Box(const glm::vec3& min, const glm::vec3& max)
	: m_Min(min), m_Max(max)
{
}

Voxel::Box::Box(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: m_Min(glm::vec3(minX, minY, minZ)), m_Max(glm::vec3(maxX, maxY, maxZ))
{
}

Voxel::Box::Box(const glm::vec3& max)
	: m_Min(glm::vec3(0)), m_Max(max)
{
}

Voxel::Box::~Box()
{
}

bool Voxel::Box::CollidesWith(Box b)
{
	const glm::vec3 max = b.GetMax();
	const glm::vec3 min = b.GetMin();

	if (m_Max.x <= min.x || m_Min.x > max.x) return false;
	if (m_Max.y <= min.y || m_Min.y > max.y) return false;
	if (m_Max.z <= min.z || m_Min.z > max.z) return false;

	return true;
}

bool Voxel::Box::CollidesWith(Box* b)
{
	return CollidesWith(*b);
}

bool Voxel::Box::CollidesWith(Mesh m)
{
	glm::vec3 min = m.GetTransForm()->GetTranslation();
	glm::vec3 max = min + (glm::vec3)m.GetMeshData()->GetSize();

	if (!CollidesWith(Box(min, max)))
	{
		return false;
	}

	// Todo: Check if the actual mesh collides with the box.
	return true;
}

bool Voxel::Box::CollidesWith(Mesh* m)
{
	return CollidesWith(*m);
}
