// Framework include
#include "Voxel/Voxel.h"

// Game class include
#include "games/test/Test.h"

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
    g_Game = new Test(g_Width, g_Height, g_Title);
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