#pragma once

#include "glm/glm.hpp"

namespace Voxel
{
	class Box;
	class Mesh;

	class Line
	{
	private:
		glm::vec3 m_Position, m_Normal;
	public:
		Line(const glm::vec3& position, const glm::vec3& normal);
		Line(const glm::vec3& normal);
		~Line();

		inline glm::vec3 GetPosition() { return m_Position; }
		inline glm::vec3 GetNormal() { return m_Normal; }

		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetNormal(const glm::vec3& normal) { m_Normal = normal; }

		bool CollidesWith(Box b);
		bool CollidesWith(Mesh m);
	};
}