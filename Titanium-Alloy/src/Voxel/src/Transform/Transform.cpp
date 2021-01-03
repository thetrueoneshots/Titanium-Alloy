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

	glm::vec3 trans = (1 - percentage) * t.GetTranslation() + percentage * m_Translation;
	glm::vec3 rot = (1 - percentage) * t.GetRotation() + percentage * m_Rotation;
	glm::vec3 scale = (1 - percentage) * t.GetScale() + percentage * m_Scale;

	tt.SetTranslation(trans);
	tt.SetRotation(rot);
	tt.SetScale(scale);

	return tt;
}

Voxel::Transform Voxel::Transform::Interpolate(Transform* t, float percentage)
{
	return Interpolate(*t, percentage);
}

Voxel::Transform Voxel::Transform::Combine(const Transform& t)
{
	Transform tt;
	tt.SetRotation(m_Rotation + t.GetRotation());
	tt.SetTranslation(m_Translation + t.GetTranslation());
	tt.SetScale(m_Scale + t.GetScale());

	return tt;
}

Voxel::Transform Voxel::Transform::Combine(Transform* t)
{
	return Combine(*t);
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
