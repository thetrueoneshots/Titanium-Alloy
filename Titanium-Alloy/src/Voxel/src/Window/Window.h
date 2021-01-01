#pragma once
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace Voxel
{
	//static int s_WINDOW_ANTI_ALIAS_SAMPLE_SIZE = 4;

	class Window
	{
	private:
		GLFWwindow* m_Window;
		int m_AntiAliasSampleSize;
	public:
		Window(int width, int height, const std::string& name);
		~Window();

		enum class CallbackType
		{
			ERROR,
			KEY,
			CURSOR,
			SCROLL
		};

		bool ShouldClose();
		void Close();
		double GetTime();

		void Clear() const;
		void Update();

		// See glfw callbacks for the correct parameters
		void SetCallback(CallbackType t, void* callback);
	private:
		void Init(int width, int height, const std::string& name);

	};
}