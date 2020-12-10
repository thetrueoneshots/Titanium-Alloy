#include "Frustum.h"

#include "glm/gtc/matrix_transform.hpp"

Voxel::CameraFrustum::CameraFrustum(glm::ivec2* screenSize, glm::vec3* normal, glm::vec3* position, float* fov, float* distance)
	: m_RenderDistance(distance), m_ScreenSize(screenSize), m_Normal(normal), m_Position(position), m_Fov(fov),
	m_Planes()
{
	
}

Voxel::CameraFrustum::~CameraFrustum()
{
}

static Plane CreatePlane(glm::vec4 in)
{
	glm::vec3 normal = glm::vec3(in.x, in.y, in.z);
	float magnitude = glm::length(normal);

	normal = glm::normalize(normal);

	return { normal, in.w / magnitude };
}

void Voxel::CameraFrustum::Update(const glm::mat4& vp)
{
	// Todo: Check if frustum is correct.
	/*glm::vec4 row1 = glm::vec4(vp[0][0], vp[0][1], vp[0][2], vp[0][3]);
	glm::vec4 row2 = glm::vec4(vp[1][0], vp[1][1], vp[1][2], vp[1][3]);
	glm::vec4 row3 = glm::vec4(vp[2][0], vp[2][1], vp[2][2], vp[2][3]);
	glm::vec4 row4 = glm::vec4(vp[3][0], vp[3][1], vp[3][2], vp[3][3]);*/

	/*glm::vec4 row1 = glm::vec4(vp[0]);
	glm::vec4 row2 = glm::vec4(vp[1]);
	glm::vec4 row3 = glm::vec4(vp[2]);
	glm::vec4 row4 = glm::vec4(vp[3]);*/

	/*m_Planes[0] = CreatePlane(row4 + row1);
	m_Planes[1] = CreatePlane(row4 - row1);
	m_Planes[2] = CreatePlane(row4 + row2);
	m_Planes[3] = CreatePlane(row4 - row2);
	m_Planes[4] = CreatePlane(row4 + row3);
	m_Planes[5] = CreatePlane(row4 - row3);*/

	m_Planes[0] = { *m_Normal, -glm::dot(*m_Position, *m_Normal) };
	m_Planes[1] = { -*m_Normal, -glm::dot(*m_Position + *m_Normal * (*m_RenderDistance), -*m_Normal) };
}

bool Voxel::CameraFrustum::IsInFrustum(const glm::vec3& pos, float radius)
{
	static int max = 6;
	for (int i = 0; i < 6 && i < max; i++)
	{
		if (glm::dot(m_Planes[i].normal, pos) + m_Planes[i].distance + 1.42f * radius < 0)
		{
			return false;
		}
	}
	
	return true;
}
