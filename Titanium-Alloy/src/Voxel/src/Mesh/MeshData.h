#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "DataTypes.h"
#include "Cube.h"

namespace Voxel
{
	/*
	* Struct for storing the color of a block.
	* Todo: Convert the floats to chars to save data storage.
	*/
	struct Color
	{
		float r, g, b, a;

		Color(const glm::vec4& color)
		{
			this->r = color.r;
			this->g = color.g;
			this->b = color.b;
			this->a = color.a;
		}

		Color(float r, float g, float b, float a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color()
		{
			this->r = 0;
			this->g = 0;
			this->b = 0;
			this->a = 0;
		}
	};

	/*
	* Class containing block data for a mesh. This is a seperate class, to
	* allow multiple similar meshes with different transforms.
	* Todo: Consider changing width, depth and height to chars to save data.
	*/
	class MeshData
	{
	private:
		int m_Width, m_Height, m_Depth;
		Color*** m_Colors;
		bool m_Updated;
		RenderData* m_Cache;
	public:
		/*
		* Constructor and deconstructor
		*/
		MeshData(unsigned int width, unsigned int height, unsigned int depth);
		MeshData(const glm::ivec3& size);
		~MeshData();

		/*
		* Getters
		*/
		inline glm::ivec3 GetSize() { return glm::ivec3(m_Width, m_Height, m_Depth); }
		inline Color*** GetColors() { return m_Colors; }
		inline RenderData* GetCache() { return m_Cache; }
		inline bool GetUpdated() { return m_Updated; }

		/*
		* Setters
		* Todo: Implement when needed.
		*/
		void SetSize(unsigned int width, unsigned int height, unsigned int depth);
		void SetSize(const glm::ivec3& size);
		void SetColors(Color*** colors);
		void SetCache(RenderData* cache);
		void SetUpdated(bool updated);

		/*
		* Logic
		*/
		void AddCube(const glm::ivec3& position, const glm::vec4& color);
		void AddCube(unsigned int x, unsigned int y, unsigned int z, const glm::vec4& color);
		void SetData(const std::vector<Cube*>& cubes);
		RenderData* CalculateRenderData();

	private:
		void Init(unsigned int width, unsigned int height, unsigned int depth);
		unsigned char GetConnectedBlockFlags(const glm::vec3& pos);
		int CheckBlock(unsigned int x, unsigned int y, unsigned int z);
		void DeleteCache();
	};
}