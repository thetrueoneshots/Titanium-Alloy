#pragma once

#include "glm/glm.hpp"

class Camera
{
private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;

public:
    Camera() : m_Position(glm::vec3(0)), m_Rotation(glm::vec3(0)) {}
    ~Camera() {}

    void SetPosition(glm::vec3 position);
    void Move(glm::vec3 direction);

    void SetRotation(glm::vec3 rotation);
    void Rotate(glm::vec3 rotation);

    glm::mat4 GetViewMatrix() const;
};