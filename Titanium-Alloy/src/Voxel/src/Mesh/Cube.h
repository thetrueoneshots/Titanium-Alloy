#pragma once

#include <vector>

#include "DataTypes.h"

#include "Quad.h"

namespace Voxel
{
	/*
	* Represents a single cube in a mesh.
	*/
	class Cube
	{
	private:
		// Todo: Change to 3 chars (0 - 255)
		glm::vec3 m_Position;

		// Todo: Change to 4 chars (0 - 255)
		glm::vec4 m_Color;

		/*
		* m_Flags represents the presence of neighbouring cubes.
		* Used for render optimizations, not rendering quads that are not visible.
		*/
		unsigned char m_Flags;
		
		//Todo: Cache quads
		//Todo: Add state (updated)

	public:
		/*
		* Constructor and deconstructor
		*/
		Cube(glm::vec3 position, glm::vec4 color, unsigned char flags = 0)
			: m_Position(position), m_Color(color), m_Flags(flags) {}
		~Cube() {}

		/*
		* Getters
		*/
		glm::vec4 GetColor() const;
		glm::vec3 GetPosition() const;

		/*
		* Setters
		*/
		void SetFlags(unsigned char flags);

		/*
		* Calculates the required quads to draw for this cube
		*/
		std::vector<Quad> GetQuads() const;
	};
}