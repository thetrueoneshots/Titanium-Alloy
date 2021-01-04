#pragma once

#include "glm/glm.hpp"

#include "../Mesh/Mesh.h"

namespace Voxel
{
	/*
	Class for calculating box collisions.
	*/
	class Box
	{
	private:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	public:
		/*
		Constructors and deconstructor.
		*/
		Box(const glm::vec3& min, const glm::vec3& max);
		Box(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		Box(const glm::vec3& max);
		~Box();

		/*
		Getters.
		*/
		inline glm::vec3 GetMin() { return m_Min; }
		inline glm::vec3 GetMax() { return m_Max; }

		/*
		Setters.
		*/
		inline void SetMin(const glm::vec3& min) { m_Min = min; }
		inline void SetMax(const glm::vec3& max) { m_Max = max; }

		/*
		Logic.
		*/
		bool CollidesWith(Box b);
		bool CollidesWith(Box* b);

		bool CollidesWith(Mesh m);
		bool CollidesWith(Mesh* m);
	};
}