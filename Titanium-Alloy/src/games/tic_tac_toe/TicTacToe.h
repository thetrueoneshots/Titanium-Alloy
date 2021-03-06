#pragma once

#include <map>

#include "../Voxel/Voxel.h"

class TicTacToe : public Voxel::Game
{
public:
	// Framework defined functions
	TicTacToe(unsigned int width, unsigned int height, const std::string& title) : Voxel::Game(width, height, title) { Init(); }

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
	enum class ModelType
	{
		CROSS = 0,
		CIRCLE,
		WINNER,
		LOSER,
		SELECTOR,
		BOARD,
	};

	std::map<int, Voxel::MeshData*> m_Models;

	bool m_FirstMouse;
	double m_LastTime = 0.0f;
	glm::vec2 m_LastMouse = glm::vec2(0, 0);
	char m_Grid[3][3];
	bool m_Player1 = true;

	glm::ivec2 m_Selector;

	void ResetGrid();
};