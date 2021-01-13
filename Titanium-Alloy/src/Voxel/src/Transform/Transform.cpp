#include "Transform.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

Voxel::Transform::Transform()
	: m_Translation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f)),
	m_Cache(glm::mat4(1.0f)), m_Updated(false)
{

}

Voxel::Transform::~Transform()
{
}

/*
* Interpolates both transforms based on the percentage.
* Note: If fullRotations is disabled, a rotation of 360 degrees or more is capped back to a single rotation.
*/
Voxel::Transform Voxel::Transform::Interpolate(const Transform& t, float percentage, bool fullRotations)
{
	Transform tt;

	glm::vec3 trans = (1 - percentage) * t.GetTranslation() + percentage * m_Translation;

	glm::vec3 rot;

	if (!fullRotations)
	{
		float twoPi = 6.28f;
		glm::vec3 moduloOldRotation = glm::vec3(
			std::fmod(m_Rotation.x, twoPi),
			std::fmod(m_Rotation.y, twoPi),
			std::fmod(m_Rotation.z, twoPi)
		);

		rot = (1 - percentage) * t.GetRotation() + percentage * moduloOldRotation;
	}
	else
	{
		rot = (1 - percentage) * t.GetRotation() + percentage * m_Rotation;
	}
	
	glm::vec3 scale = (1 - percentage) * t.GetScale() + percentage * m_Scale;

	tt.SetTranslation(trans);
	tt.SetRotation(rot);
	tt.SetScale(scale);

	return tt;
}

Voxel::Transform Voxel::Transform::Interpolate(Transform* t, float percentage, bool fullRotations)
{
	return Interpolate(*t, percentage, fullRotations);
}

Voxel::Transform Voxel::Transform::Combine(const Transform& t)
{
	Transform tt;
	tt.SetRotation(m_Rotation + t.GetRotation());
	tt.SetTranslation(m_Translation + t.GetTranslation());
	tt.SetScale(m_Scale * t.GetScale());

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
	glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
	m_Cache = glm::scale(trans * rotation, m_Scale);
	m_Updated = false;

	return m_Cache;
}
