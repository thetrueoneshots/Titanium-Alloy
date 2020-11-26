#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

#include <math.h> 

void Camera::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

void Camera::Move(Direction direction, float speed)
{
	switch (direction)
	{
	case Direction::FORWARD:
		m_Position += speed * m_Normal;
		break;
	case Direction::BACKWARD:
		m_Position -= speed * m_Normal;
		break;
	case Direction::LEFT:
		m_Position -= speed * glm::normalize(glm::cross(m_Normal, m_ViewRotation));
		break;
	case Direction::RIGHT:
		m_Position += speed * glm::normalize(glm::cross(m_Normal, m_ViewRotation));
		break;
	case Direction::UP:
		m_Position += speed * m_ViewRotation;
		break;
	case Direction::DOWN:
		m_Position -= speed * m_ViewRotation;
		break;
	default:
		break;
	}
}

void Camera::Mouse(glm::vec2 offset)
{
	m_Yaw += offset.x;
	m_Pitch += offset.y;

	m_Pitch = m_Pitch > 89.0f ? 89.0f : m_Pitch < -89.0f ? -89.0f : m_Pitch;

	m_Normal = glm::normalize(glm::vec3(
		cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
		sin(glm::radians(m_Pitch)),
		sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
	));
}

void Camera::Scroll(double yOffset)
{
	if (m_Fov >= 1.0f && m_Fov <= 45.0f)
	{
		m_Fov -= yOffset;
	}
	if (m_Fov <= 1.0f)
	{
		m_Fov = 1.0f;
	}
	if (m_Fov >= 45.0f)
	{
		m_Fov = 45.0f;
	}
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(
		glm::radians(m_Fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90� (extra wide) and 30� (quite zoomed in)
		(float)m_ScreenSize->x / (float)m_ScreenSize->y, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		100.0f             // Far clipping plane. Keep as little as possible.
	);
}

// Todo: Add rotation
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(
		m_Position,
		m_Position + m_Normal,
		m_ViewRotation
	);
}
