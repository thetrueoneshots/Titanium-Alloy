#pragma once

#include <string>
#include "glm/glm.hpp"

namespace Voxel
{
	/*
* Struct for returning all the calculated (OpenGL) render data
*/
	struct RenderData
	{
		float* vertices;
		size_t vertex_array_size;
		unsigned int* indices;
		size_t indices_array_count;
		RenderData(float* v, size_t s1, unsigned int* i, size_t s2)
		{
			vertices = v;
			vertex_array_size = s1;
			indices = i;
			indices_array_count = s2;
		}
	};

	struct Vertex
	{
		int32_t position;
		int32_t color;
		int32_t normal;

		Vertex() : position(0), color(0), normal(0) {}

		Vertex(glm::ivec3 p, glm::vec4 c, glm::vec3 n)
		{
			position = p.x + 127 | p.y + 127 << 8 | p.z + 127 << 16;
			color = (int)(255 * c.x) | (int)(255 * c.y) << 8 | (int)(255 * c.z) << 16 | (int)(255 * c.w) << 24;
			normal = (int)(255 * n.x) | (int)(255 * n.y) << 8 | (int)(255 * n.z) << 16;
		}
	};

	struct QuadData
	{
		Vertex vertices[4];
		unsigned int indices[6];

		QuadData(float v[], unsigned int i[])
		{
			memcpy(vertices, v, 4 * sizeof(Vertex));
			memcpy(indices, i, 6 * sizeof(unsigned int));
		}

		QuadData(Vertex v[], unsigned int i[])
		{
			memcpy(vertices, v, 4 * sizeof(Vertex));
			memcpy(indices, i, 6 * sizeof(unsigned int));
		}
	};
}