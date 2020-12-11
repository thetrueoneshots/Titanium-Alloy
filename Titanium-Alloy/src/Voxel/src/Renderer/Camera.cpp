#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

#include <math.h> 

Voxel::Camera::Camera(glm::ivec2* screenSize, float renderDistance)
	: m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Normal(glm::vec3(0.0f, 0.0f, -1.0f)), m_ViewRotation(CAMERA_UP),
	m_Yaw(-90.0f), m_Pitch(0.0f), m_Fov(45.0f), m_ScreenSize(screenSize), m_RenderDistance(renderDistance) 
{
	m_Frustum = new CameraFrustum(m_ScreenSize, &m_Normal, &m_Position, &m_Fov, &m_RenderDistance);
}

Voxel::Camera::~Camera()
{
	delete m_Frustum;
}

void Voxel::Camera::SetPosition(glm::vec3 position)
{
	m_Position = position;
	m_Frustum->Update(GetProjectionMatrix()*GetViewMatrix());
}

void Voxel::Camera::Move(Direction direction, float speed)
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
	m_Frustum->Update(GetProjectionMatrix() * GetViewMatrix());
}

void Voxel::Camera::Mouse(glm::vec2 offset)
{
	m_Yaw += offset.x;
	m_Pitch += offset.y;

	m_Pitch = m_Pitch > 89.0f ? 89.0f : m_Pitch < -89.0f ? -89.0f : m_Pitch;

	m_Yaw = m_Yaw >= 360.0f ? m_Yaw - 360.0f : m_Yaw <= 0 ? m_Yaw + 360.0f : m_Yaw;

	m_Normal = glm::normalize(glm::vec3(
		cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
		sin(glm::radians(m_Pitch)),
		sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
	));

	m_Frustum->Update(GetProjectionMatrix() * GetViewMatrix());
}

void Voxel::Camera::Scroll(double yOffset)
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

	m_Frustum->Update(GetProjectionMatrix() * GetViewMatrix());
}

bool Voxel::Camera::InFrustum(const glm::vec3& pos, float radius)
{
	//return glm::dot(m_Normal, pos) - glm::dot(m_Position, m_Normal) + 1.42f*radius > 0;
	return m_Frustum->IsInFrustum(pos, radius);
}

glm::mat4 Voxel::Camera::GetProjectionMatrix() const
{
	return glm::perspective(
		glm::radians(m_Fov), // The vertical Field of View, in radians: the amount of "zoom". Think "Voxel::Camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		(float)m_ScreenSize->x / (float)m_ScreenSize->y, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		m_RenderDistance            // Far clipping plane. Keep as little as possible.
	);
}

glm::mat4 Voxel::Camera::GetViewMatrix() const
{
	return glm::lookAt(
		m_Position,
		m_Position + m_Normal,
		m_ViewRotation
	);
}
