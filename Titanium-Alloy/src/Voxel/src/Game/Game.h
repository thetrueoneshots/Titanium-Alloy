#pragma once

#include "../Window/Window.h"
#include "../Renderer/Renderer.h"

namespace Voxel
{
	class Game
	{
	// Protected, because the derived class needs to be able to access them as well.
	protected:
		Window* m_Window;
		Camera* m_Camera;
		Renderer* m_Renderer;
		double m_FrameTime;
		int m_FPS;
		std::string m_Title;

	public:
		Game(const unsigned int width, const unsigned int height, const std::string& title);
		~Game();

		virtual void Init();
		virtual void Cleanup();
		virtual void Update();

		void Render();

		bool ShouldClose();
		void SetCallback(Window::CallbackType t, void* callback);
		
		// Callbacks
		virtual void ErrorCallback(int error, const char* description);
		virtual void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void CursorCallback(GLFWwindow* window, double xPos, double yPos);
		virtual void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		virtual void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	};
}