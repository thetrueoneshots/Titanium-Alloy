#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "World/World.h"

#include "../Voxel/Voxel.h"

static const std::string WINDOW_NAME = "Titanium Alloy";
static const float MOVEMENT_SPEED = 20.0f;
static const float MOUSE_SENSITIVITY = 0.1f;

class Game
{
private:
	glm::ivec2 m_ScreenSize;
	GLFWwindow* m_Window;
	double m_DeltaTime;
	double m_LastFrame;
	Voxel::Camera* m_Camera;
	Voxel::Renderer* m_Renderer;
	World* m_World;
	float m_MovementSpeed;

	bool m_FirstMouse;
	glm::vec2 m_LastMouse;
	float m_Sensitivity;
public:
	Game(GLFWwindow* window);
	~Game();

	void Update(double currentFrame);

	void HandleKeyInput(int key, int scancode, int action, int mods);
	void HandleCursorPosition(double xPos, double yPos);
	void HandleScrollInput(double xOffset, double yOffset);
};