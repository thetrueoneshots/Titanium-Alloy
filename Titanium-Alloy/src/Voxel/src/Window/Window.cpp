#include "Window.h"

Voxel::Window::Window(int width, int height, const std::string& name)
	: m_Window(nullptr)
{
	Init(width, height, name);
}

Voxel::Window::~Window()
{
	glfwDestroyWindow(m_Window);

	glfwTerminate();
}

bool Voxel::Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Voxel::Window::Close()
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

double Voxel::Window::GetTime()
{
	return glfwGetTime();
}

void Voxel::Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Voxel::Window::SetCallback(CallbackType t, void* callback)
{
    /*glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    //glfwSetCursorPosCallBack(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);*/
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
    }
}

void Voxel::Window::Init(int width, int height, const std::string& name)
{
    const static int ANTI_ALIAS_SAMPLE_SIZE = 4;

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

    
    /*glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    //glfwSetCursorPosCallBack(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);*/

    // Maybe GLFW_CURSOR_HIDDEN?
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(m_Window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}

