#pragma once

#include <vector>
#include <map>
#include <utility>

#include "glm/glm.hpp"

#include "Cube.h"
#include "Quad.h"

class Mesh
{
private:
	static enum class State
	{
		STATE_BEGIN,
		UPDATED = 0,
		STATE_END
	};

	glm::vec3 m_Translation, m_Scale, m_Rotation;
	//std::vector<Cube*> m_Cubes;
	std::map<std::pair<int, std::pair<int, int>>, Cube*> m_Cubes;
	unsigned char m_State;
	std::vector<Quad> m_Cache;
public:
	Mesh();
	~Mesh();

	Mesh(std::vector<Cube*> cubes);

	void AddCube(glm::vec3 position, glm::vec4 color, unsigned char flags = 0);
	void AddCube(float p1, float p2, float p3, glm::vec4 color, unsigned char flags = 0);

	void SetTranslation(const glm::vec3& translation);
	void Translate(const glm::vec3& translation);
	void SetScale(float scale);
	void SetScale(const glm::vec3& scale);
	// Rotation is radians!
	void Rotate(const glm::vec3& rotation);
	// Rotation is radians!
	void SetRotation(const glm::vec3& rotation);

	std::vector<Quad> GetQuads();

	void UpdateRenderFlags();
	glm::mat4 GetModelMatrix() const;

private:
	// Use State enums 
	bool GetState(State s) const;
	bool SetState(State s);
	bool UnsetState(State s);

	unsigned char GetConnectedBlockFlags(const glm::vec3& pos);
	int CheckBlock(int x, int y, int z);
};