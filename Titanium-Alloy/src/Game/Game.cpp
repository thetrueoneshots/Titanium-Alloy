#include "Game.h"

Game::Game(GLFWwindow* window)
	:m_DeltaTime(0.0f), m_LastFrame(0.0f), 
	m_Camera(nullptr), m_Renderer(nullptr), m_World(nullptr), m_Window(window),
	m_MovementSpeed(MOVEMENT_SPEED), m_FirstMouse(true), m_Sensitivity(MOUSE_SENSITIVITY)
{
	glfwGetWindowSize(m_Window, &m_ScreenSize.x, &m_ScreenSize.y);
	m_LastMouse = glm::vec2(m_ScreenSize.x / 2, m_ScreenSize.y / 2);
	m_Camera = new Camera(&m_ScreenSize, 500.0f);
	m_Camera->SetPosition(glm::vec3(0.0f, 15.0f, -20.0f));
	m_Renderer = new Renderer(m_Camera);
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

void Game::Update(float currentFrame)
{
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;
	m_Renderer->Clear();
	
	m_Renderer->Update();
	m_World->Update();
}

void Game::HandleKeyInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);

	float cameraSpeed = m_MovementSpeed * m_DeltaTime;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
		m_Camera->Move(Camera::Direction::LEFT, cameraSpeed);
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
		m_Camera->Move(Camera::Direction::RIGHT, cameraSpeed);
	if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
		m_Camera->Move(Camera::Direction::FORWARD, cameraSpeed);
	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
		m_Camera->Move(Camera::Direction::BACKWARD, cameraSpeed);
	if (key == GLFW_KEY_LEFT_SHIFT)
		m_Camera->Move(Camera::Direction::DOWN, cameraSpeed);
	if (key == GLFW_KEY_SPACE)
		m_Camera->Move(Camera::Direction::UP, cameraSpeed);
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
