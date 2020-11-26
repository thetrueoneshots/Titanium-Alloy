#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

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

#include "World/World.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static int ANTI_ALIAS_SAMPLE_SIZE = 4;

const int WIDTH = 640;
const int HEIGHT = 480;
static Renderer renderer(WIDTH, HEIGHT);

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* InitGlfwWindow(const unsigned int width, const unsigned int height, const std::string& title);

// Todo: Move camera.
// Todo: Make screen resizing work.
// Todo: Optimize mesh code.
int main(void)
{
    // Create window and setup glfw context.
    GLFWwindow* window = InitGlfwWindow(WIDTH, HEIGHT, "Engine Window");

    renderer.Init();
    renderer.GetCamera()->SetPosition(glm::vec3(5.0f, 50.0f, 50.0f));

    World* world = new World(&renderer);
    world->GenerateWorld();
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        renderer.Update();
        
        world->Update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete world;

    // Cleanup.
    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Todo: Implement actual moving
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_LEFT)
        renderer.GetCamera()->Move(glm::vec3(-1.0f, 0.0f, 0.0f));
    if (key == GLFW_KEY_RIGHT)
        renderer.GetCamera()->Move(glm::vec3(1.0f, 0.0f, 0.0f));
    if (key == GLFW_KEY_UP)
        renderer.GetCamera()->Move(glm::vec3(0.0f, 1.0f, 0.0f));
    if (key == GLFW_KEY_DOWN)
        renderer.GetCamera()->Move(glm::vec3(0.0f, -1.0f, 0.0f));
}

GLFWwindow* InitGlfwWindow(const unsigned int width, const unsigned int height, const std::string& title)
{
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

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    return window;
}