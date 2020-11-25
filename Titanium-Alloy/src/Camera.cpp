#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

void Camera::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Camera::Move(glm::vec3 direction)
{
	m_Position += direction;
}

void Camera::SetRotation(glm::vec3 rotation)
{
	m_Rotation = rotation;
}

void Camera::Rotate(glm::vec3 rotation)
{
	m_Rotation += rotation;
}

// Todo: Add rotation
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(
		m_Position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f) //To look upside down or not. (Handy for shaking maybe)
	);
}
