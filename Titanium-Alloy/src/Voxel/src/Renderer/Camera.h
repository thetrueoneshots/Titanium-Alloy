#pragma once

#include "Frustum.h"
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
        CameraFrustum* m_Frustum;

        // Todo: Cache view / projection matrix and update on variable updates
        // Todo: Think about abtracting the camera away from visible framework code
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

        Camera(glm::ivec2* screenSize, float renderDistance = 100.0f);
        ~Camera();

        /*
        * Getters
        */
        inline glm::vec3 GetPosition() const { return m_Position; }

        /*
        * Setters
        */
        // Todo: Rename to [ SetTranslation(const glm::vec3& translation) ]
        void SetPosition(glm::vec3 position);
        // Todo: Rename to [ Translate(Direction direction, float speed) ]
        void Move(Direction direction, float speed);
        // Todo: Implement
        void Translate(const glm::vec3& translation);

        // Todo: Rename to [ Rotate(float yaw, float pitch) ]
        void Mouse(glm::vec2 offset);
        // Todo: Implement
        void SetRotation(const glm::vec3& rotation);
        // Todo: Implement
        void Rotate(const glm::vec3& rotation);

        // Todo: Rename to [ Zoom(float change) | ChangeFieldOfView(float change) ]
        void Scroll(double yOffset);
        // Todo: Implement and optionally rename [ SetFieldOfView(float FoV) ]
        void SetZoom(float zoom);

        bool InFrustum(const glm::vec3& pos, float radius);

        // Todo: Rename to [ ProjectionMatrix() | CalculateProjectionMatrix() ]
        glm::mat4 GetProjectionMatrix() const;

        // Todo: Rename to [ ViewMatrix() | CalculateViewMatrix() ]
        glm::mat4 GetViewMatrix() const;
    };
}