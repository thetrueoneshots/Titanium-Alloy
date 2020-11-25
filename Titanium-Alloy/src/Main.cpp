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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "noise/fast_noise_lite.hpp"

static const glm::vec4 COLOR_BROWN = glm::vec4(0.54f, 0.27f, 0.07f, 1.0f);
static const glm::vec4 COLOR_DARK_GREEN = glm::vec4(0.12f, 0.51f, 0.28f, 1.0f);
static const glm::vec4 COLOR_GREEN = glm::vec4(0.22f, 0.71f, 0.38f, 1.0f);

static int ANTI_ALIAS_SAMPLE_SIZE = 4;

const int WIDTH = 640;
const int HEIGHT = 480;
static Renderer renderer(WIDTH, HEIGHT);

// Todo: Temp
Mesh CreateTree()
{

    Mesh m;
    // Base
    for (int i = 0; i < 3; i++)
    {
        m.AddCube(0.0f, i * 1.0f, 0.0f, COLOR_BROWN);
    }

    //Leafs
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                m.AddCube(-1.0f + j * 1.0f, 3.0f + i, -1.0f + k * 1.0f, COLOR_DARK_GREEN);
            }
        }
    }
    
    return m;
}

// Todo: Temp
Mesh CreateChunk(int x, int y)
{
    static int SIZE = 16;
    static int NOISE_MULTIPLIER = 5;
    Mesh m;
    m.SetPosition(glm::vec3((float)x * SIZE, 0.0f, (float)y * SIZE));

    FastNoiseLite noise(0);
    noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);

    for (int i = -SIZE; i < SIZE; i++)
    {
        for (int j = -SIZE; j < SIZE; j++)
        {
            float height = noise.GetNoise((float)(i + x * SIZE)  * NOISE_MULTIPLIER, (float)(j + y* SIZE) * NOISE_MULTIPLIER);
            float yPos = (int)(height * 20.0f) - 5.0f;
            glm::vec4 tint = glm::vec4(glm::vec3(0.3f, 0.3f, 0.3f) * height, 0.0f);
            m.AddCube(i * 1.0f, yPos - 1.0f, j * 1.0f, COLOR_GREEN + tint);
            m.AddCube(i * 1.0f, yPos, j * 1.0f, COLOR_GREEN + tint);
        }
    }
    return m;
}

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* InitGlfwWindow(const unsigned int width, const unsigned int height, const std::string& title);

// Todo: Create a github repo.
// Todo: Move camera.
// Todo: Make screen resizing work.
// Todo: Optimize mesh code.

int main(void)
{
    // Create window and setup glfw context.
    GLFWwindow* window = InitGlfwWindow(WIDTH, HEIGHT, "Engine Window");

    renderer.Init();
    renderer.GetCamera()->SetPosition(glm::vec3(5.0f, 50.0f, 50.0f));

    
    // Todo: Move MVP to Renderer::Draw
    /*float FoV = 45.0f;
    
    // Projection
    glm::mat4 proj = glm::perspective(
        glm::radians(FoV), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        1.0f * WIDTH / HEIGHT, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        200.0f             // Far clipping plane. Keep as little as possible.
    );

    // View
    glm::mat4 view = glm::lookAt(
        glm::vec3(5.0f, 50.0f, 50.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    // Model
    glm::mat4 model = glm::mat4(1.0f);

    // MVP matrix
    glm::mat4 mvp = proj * view * model; */

    Shader shader("res/shaders/Basic.shader");

    // Only for texture shaders
    /*Texture texture("res/image/texture.png");
    texture.Bind();
    shader.SetUniform<int>("u_Texture", 0);*/

    Mesh m = CreateTree();

    // Todo: Create chunk class
    static int CHUNKS = 2;
    std::vector<Mesh> chunks;
    for (int i = 0; i < CHUNKS; i++)
    {
        for (int j = 0; j < CHUNKS; j++)
        {
            chunks.push_back(CreateChunk(i - CHUNKS / 2, j - CHUNKS /2));
        }
    }

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        renderer.Update();
        
        for (Mesh m1 : chunks)
        {
            //shader.Bind();
            //glm::mat4 temp = glm::translate(mvp, m1.GetPosition());
            //shader.SetUniform<glm::mat4>("u_MVP", temp);

            renderer.DrawMesh(m1, shader);
        }

        //shader.Bind();
        //shader.SetUniform<glm::mat4>("u_MVP", mvp);

        //renderer.DrawMesh(m, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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