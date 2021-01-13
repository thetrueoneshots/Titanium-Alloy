#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include <string>

namespace Voxel
{
	//static int s_WINDOW_ANTI_ALIAS_SAMPLE_SIZE = 4;

	class Window
	{
	private:
		GLFWwindow* m_Window;
	public:
		Window();
		Window(int width, int height, const std::string& name);
		~Window();

		enum class CallbackType
		{
			ERROR,
			KEY,
			CURSOR,
			SCROLL,
			MOUSE_BUTTON,
		};

		bool ShouldClose();
		void Close();

		void SetTitle(const std::string& title);
		void SetBackgroundColor(const glm::vec3& color);

		double GetTime();

		inline GLFWwindow* GetGlfwWindow() { return m_Window; }

		void Clear() const;
		void Update();

		// See glfw callbacks for the correct parameters
		void SetCallback(CallbackType t, void* callback);
	private:
		void Init(int width, int height, const std::string& name);

	};
}