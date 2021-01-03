#include "Transform.h"

#include "glm/gtc/matrix_transform.hpp"

Voxel::Transform::Transform()
	: m_Translation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f)),
	m_Cache(glm::mat4(1.0f)), m_Updated(false)
{

}

Voxel::Transform::~Transform()
{
}

Voxel::Transform Voxel::Transform::Interpolate(const Transform& t, float percentage)
{
	Transform tt;

	glm::vec3 trans = percentage * t.GetTranslation() + (1 - percentage) * m_Translation;
	glm::vec3 rot = percentage * t.GetRotation() + (1 - percentage) * m_Rotation;
	glm::vec3 scale = percentage * t.GetScale() + (1 - percentage) * m_Scale;

	tt.SetTranslation(trans);
	tt.SetRotation(rot);
	tt.SetScale(scale);

	return tt;
}

Voxel::Transform Voxel::Transform::Interpolate(Transform* t, float percentage)
{
	return Interpolate(*t, percentage);
}

glm::mat4 Voxel::Transform::CalculateModelMatrix()
{
	if (!m_Updated)
	{
		return m_Cache;
	}

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_Translation);
	glm::mat4 rx = glm::rotate(trans, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(rx, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(ry, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	m_Cache = glm::scale(rz, m_Scale);
	m_Updated = false;

	return m_Cache;
}
