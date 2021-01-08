# Voxel Framework Readme

## Introduction
Welcome to everyone reading this readme. In this readme will explain the functionalities of the framework and contains a small tutorial on how to setup the framework. 

## Functionalities
This framwork contains the following functionalities:
- It can render voxel meshes as single instances, but a mesh can also be batch rendered with multiple transformations.
- It supports the usage of different rendercontexts (OpenGL Shaders, buffers, e.t.c.)
- It contains camera control
- It _will_ contain frustum culling and depth culling

## Tutorial
To start rendering voxels, you will have to have use the following setup for your code.

### The main file
In the main file, all the important initalizations are done and the GLFW callbacks are registered. The only times you want to change this file is when you are adding a new and unsupported GLFW callback type to the game or when you want to assign a new game to render.
```c++
// Framework include
#include "Voxel/Voxel.h"

// Game class include
#include "Game/GameName.h"

// Constants for the screensize
const int g_Width = 1280;
const int g_Height = 720;
const char* g_Title = "Engine window";

Voxel::Game* g_Game;

// Callback function definitions
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main(void)
{
    // Initialization
    g_Game = new GameName(g_Width, g_Height, g_Title);
    g_Game->SetCallback(Voxel::Window::CallbackType::ERROR, error_callback);
    g_Game->SetCallback(Voxel::Window::CallbackType::KEY, key_callback);
    g_Game->SetCallback(Voxel::Window::CallbackType::SCROLL, scroll_callback);
    g_Game->SetCallback(Voxel::Window::CallbackType::CURSOR, cursor_callback);
    g_Game->SetCallback(Voxel::Window::CallbackType::MOUSE_BUTTON, mouse_button_callback);

    // Game loop
    while (!g_Game->ShouldClose())
    {
        g_Game->Render();
    }

    // Cleanup
    g_Game->Cleanup();
    delete g_Game;
    exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    g_Game->KeyCallback(w, key, scancode, action, mods);
}

void cursor_callback(GLFWwindow* w, double xPos, double yPos)
{
    g_Game->CursorCallback(w, xPos, yPos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    g_Game->MouseButtonCallback(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    g_Game->ScrollCallback(window, xOffset, yOffset);
}

void error_callback(int error, const char* description)
{
    g_Game->ErrorCallback(error, description);
}
```

### Your game class
Then, the definition of your game class. Your game class will publicly extend the Voxel::Game class, such that all functions needed to run a game are 
already defined. You can overwrite the functions in your own class and implement them to your liking.
```c++
#pragma once

// Including the framework. Path may differ depending on where your framework and your own game class is.
#include "../Voxel/Voxel.h"

class GameName : public Voxel::Game
{
public:
	GameName(unsigned int width, unsigned int height, const std::string& title) : Voxel::Game(width, height, title) { Init(); }

  // Framework defined functions
	void Init();
	void Cleanup();
	void Update();

	void ErrorCallback(int error, const char* description);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CursorCallback(GLFWwindow* window, double xPos, double yPos);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
	// All user defined variables and functions
  
};
```

### Rendering your first mesh
Once you have your main and game class set up, you can start thinking about rendering.
In the update function of your game class, you can create a mesh and render it.

```c++
void GameName::Update()
{
  Voxel::Mesh mesh(1, 1, 1);
  mesh.GetMeshData()->AddCube(glm::ivec3(0, 0, 0), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  m_Renderer->Render(&mesh);
}
```
### Tips
Here are some times that I found nice while using the framework

- Creating an enum for mesh types and storing all the meshdata for all meshes in a map, at the index of the value of the enum type.
