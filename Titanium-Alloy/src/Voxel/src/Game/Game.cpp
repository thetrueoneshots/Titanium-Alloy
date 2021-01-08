#include "Game.h"

#include <iostream>

/*
* Should not be overridden! Use `init` if you want to add variables to the game.
*/
Voxel::Game::Game(unsigned int width, unsigned int height, const std::string& title)
	: m_FPS(0), m_FrameTime(0.0f), m_Title(title)
{
	m_Window = new Window(width, height, title);
	m_Camera = new Camera(glm::ivec2(width, height));
	m_Renderer = new Renderer(m_Camera);
	m_Renderer->Init();
}

/*
* Should not be overridden! Use `cleanup` if you want to delete things
*/
Voxel::Game::~Game()
{
	delete m_Window;
	delete m_Camera;
	delete m_Renderer;
}

/*
* Can be be overridden and overloaded
*/
void Voxel::Game::Init()
{
	std::cout << "Voxel::Game init" << std::endl;
}

/*
* Can be be overridden!
*/
void Voxel::Game::Cleanup()
{
}

/*
* Can be be overridden!
*/
void Voxel::Game::Update()
{
}

void Voxel::Game::Render()
{
	m_Window->Clear();
	m_Renderer->Update();

	static int count = 0;
	static double time = 0.0f;
	static double last_time = 0.0f;

	double curr_time = m_Window->GetTime();
	m_FrameTime = curr_time - last_time;
	last_time = curr_time;

	time += m_FrameTime;
	count++;

	if (time > 1.0f)
	{
		m_FPS = count;

		time = 0.0f;
		count = 0;
	}

	Update();

	m_Window->Update();
}

bool Voxel::Game::ShouldClose()
{
	return m_Window->ShouldClose();
}

void Voxel::Game::SetCallback(Window::CallbackType t, void* callback)
{
	m_Window->SetCallback(t, callback);
}

void Voxel::Game::ErrorCallback(int error, const char* description)
{
	std::cout << "Error callback!" << std::endl;
}

void Voxel::Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "Key callback!" << std::endl;
}

void Voxel::Game::CursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	std::cout << "Cursor callback!" << std::endl;
}

void Voxel::Game::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "MouseButton callback!" << std::endl;
}

void Voxel::Game::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << "ScrollCallback callback!" << std::endl;
}
