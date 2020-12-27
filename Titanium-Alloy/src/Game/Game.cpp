#include "Game.h"

#include "Geometry/MeshGenerator.h"

#include <stdio.h>

Game::Game(GLFWwindow* window)
	:m_DeltaTime(0.0f), m_LastFrame(0.0f), 
	m_Camera(nullptr), m_Renderer(nullptr), m_World(nullptr), m_Window(window),
	m_MovementSpeed(MOVEMENT_SPEED), m_FirstMouse(true), m_Sensitivity(MOUSE_SENSITIVITY)
{
	glfwGetWindowSize(m_Window, &m_ScreenSize.x, &m_ScreenSize.y);
	m_LastMouse = glm::vec2(m_ScreenSize.x / 2, m_ScreenSize.y / 2);

	m_Camera = new Voxel::Camera(&m_ScreenSize, 500.0f);
	m_Camera->SetPosition(glm::vec3(0.0f, 15.0f, -20.0f));

	m_Renderer = new Voxel::Renderer(m_Camera);
	m_Renderer->Init();

	m_World = new World(m_Renderer, m_Camera);
	m_World->GenerateWorld();
}

Game::~Game()
{
	if (m_Camera) delete m_Camera;
	if (m_Renderer) delete m_Renderer;
	if (m_World) delete m_World;
}

void Game::Update(double currentFrame)
{
	static double totalTime = 0.0f;
	static int frames = 0;
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;

	totalTime += m_DeltaTime;
	frames++;
	if (totalTime >= 1.0f)
	{
		char buffer[250];
		sprintf_s(buffer, 250, "Engine Window: [fps: %d]", frames);
		glfwSetWindowTitle(m_Window, buffer);
		totalTime -= 1.0f;
		frames = 0;
	}

	m_Renderer->Clear();
	
	m_Renderer->Update();
	m_World->Update();

	std::cout << "Update finished" << std::endl;
}

void Game::HandleKeyInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);

	float cameraSpeed = m_MovementSpeed * m_DeltaTime;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
		m_Camera->Move(Voxel::Camera::Direction::LEFT, cameraSpeed);
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
		m_Camera->Move(Voxel::Camera::Direction::RIGHT, cameraSpeed);
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
		m_Camera->Move(Voxel::Camera::Direction::FORWARD, cameraSpeed);
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
		m_Camera->Move(Voxel::Camera::Direction::BACKWARD, cameraSpeed);
	if (key == GLFW_KEY_LEFT_SHIFT)
		m_Camera->Move(Voxel::Camera::Direction::DOWN, cameraSpeed);
	if (key == GLFW_KEY_SPACE)
		m_Camera->Move(Voxel::Camera::Direction::UP, cameraSpeed);
}

void Game::HandleCursorPosition(double xPos, double yPos)
{
	if (m_FirstMouse)
	{
		m_LastMouse = glm::vec2(xPos, yPos);
		m_FirstMouse = false;
	}

	glm::vec2 offset = glm::vec2(xPos - m_LastMouse.x, m_LastMouse.y - yPos); // Y is reversed. (Y goes from top to bottom)
	m_LastMouse = glm::vec2(xPos, yPos);

	offset *= m_Sensitivity;
	
	m_Camera->Mouse(offset);
}

void Game::HandleScrollInput(double xOffset, double yOffset)
{
	m_Camera->Scroll(yOffset);
}