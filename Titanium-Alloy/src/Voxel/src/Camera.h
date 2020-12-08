#pragma once

#include "glm/glm.hpp"

const static glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

namespace Voxel
{
    class Camera
    {
    private:
        glm::vec3 m_Position, m_Normal, m_ViewRotation;
        float m_Yaw, m_Pitch, m_Fov, m_RenderDistance;
        glm::ivec2* m_ScreenSize;
    public:
        enum class Direction
        {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN,
        };

        Camera(glm::ivec2* screenSize, float renderDistance = 100.0f)
            : m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Normal(glm::vec3(0.0f, 0.0f, -1.0f)), m_ViewRotation(CAMERA_UP),
            m_Yaw(-90.0f), m_Pitch(0.0f), m_Fov(45.0f), m_ScreenSize(screenSize), m_RenderDistance(renderDistance) {}
        ~Camera() {}

        void SetPosition(glm::vec3 position);
        void Move(Direction direction, float speed);

        void Mouse(glm::vec2 offset);
        void Scroll(double yOffset);

        bool InFrustum(const glm::vec3& pos, float radius);

        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;

        inline glm::vec3 GetPosition() const { return m_Position; }
    };
}