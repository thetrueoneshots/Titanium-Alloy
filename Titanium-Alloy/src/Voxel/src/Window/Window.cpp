#include "Window.h"

#include "glm/glm.hpp"

Voxel::Window::Window()
    : m_Window(nullptr)
{
}

Voxel::Window::Window(int width, int height, const std::string& name)
	: m_Window(nullptr)
{
	Init(width, height, name);
}

Voxel::Window::~Window()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);

        glfwTerminate();
    }
}

bool Voxel::Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Voxel::Window::Close()
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void Voxel::Window::SetTitle(const std::string& title)
{
    glfwSetWindowTitle(m_Window, title.c_str());
}

void Voxel::Window::SetBackgroundColor(const glm::vec3& color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
}

double Voxel::Window::GetTime()
{
	return glfwGetTime();
}

void Voxel::Window::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Voxel::Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Voxel::Window::SetCallback(CallbackType t, void* callback)
{
    switch (t) {
    case CallbackType::ERROR:
        glfwSetErrorCallback((GLFWerrorfun)callback);
        break;
    case CallbackType::KEY:
        glfwSetKeyCallback(m_Window, (GLFWkeyfun)callback);
        break;
    case CallbackType::CURSOR:
        glfwSetCursorPosCallback(m_Window, (GLFWcursorposfun)callback);
        break;
    case CallbackType::SCROLL:
        glfwSetScrollCallback(m_Window, (GLFWscrollfun)callback);
        break;
    case CallbackType::MOUSE_BUTTON:
        glfwSetMouseButtonCallback(m_Window, (GLFWmousebuttonfun)callback);
    }
}

void Voxel::Window::Init(int width, int height, const std::string& name)
{
    const static int ANTI_ALIAS_SAMPLE_SIZE = 4;
    const static glm::vec4 s_ClearColor = glm::vec4(204 / 255.0f, 204 / 255.0f, 255 / 255.0f, 1.0f);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, ANTI_ALIAS_SAMPLE_SIZE);

    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Maybe GLFW_CURSOR_HIDDEN?
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(m_Window);
    gladLoadGL(glfwGetProcAddress);

    // Todo: Create function for this and set in game init.
    glfwSwapInterval(1);

    glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
}

