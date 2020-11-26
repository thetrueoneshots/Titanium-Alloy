#include <cstddef>
#include <linmath.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Geometry/Mesh.h"

#include "Game/Game.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const int WIDTH = 640;
const int HEIGHT = 480;

static Game* game;

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_callback(GLFWwindow* window, double xPos, double yPos);
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
GLFWwindow* InitGlfwWindow(const unsigned int width, const unsigned int height, const std::string& title);

// Todo: Move camera.
// Todo: Make screen resizing work.
// Todo: Optimize mesh code.
int main(void)
{
    // Create window and setup glfw context.
    GLFWwindow* window = InitGlfwWindow(WIDTH, HEIGHT, "Engine Window");
    game = new Game(window);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        game->Update(glfwGetTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete game;

    // Cleanup.
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    game->HandleKeyInput(key, scancode, action, mods);
}

static void cursor_callback(GLFWwindow* window, double xPos, double yPos)
{
    game->HandleCursorPosition(xPos, yPos);
}

static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    game->HandleScrollInput(xOffset, yOffset);
}

GLFWwindow* InitGlfwWindow(const unsigned int width, const unsigned int height, const std::string& title)
{
    const static int ANTI_ALIAS_SAMPLE_SIZE = 4;

    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, ANTI_ALIAS_SAMPLE_SIZE);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    //glfwSetCursorPosCallBack(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Maybe GLFW_CURSOR_HIDDEN?
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    return window;
}