#include "MeshData.h"

/*
* Constructor with 3 seperate integers.
*/
Voxel::MeshData::MeshData(unsigned int width, unsigned int height, unsigned int depth)
{
	Init(width, height, depth);
}

/*
* Constructor with a vector, sometimes easier to use if a size vector is available.
*/
Voxel::MeshData::MeshData(const glm::ivec3& size)
{
	Init(size.x, size.y, size.z);
}

/*
* Deconstructor. Deletes all allocated color data and possibly cache.
*/
Voxel::MeshData::~MeshData()
{
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			delete[] m_Colors[i][j];
		}
		delete[] m_Colors[i];
	}
	delete[] m_Colors;

	DeleteCache();
}

void Voxel::MeshData::AddCube(const glm::ivec3& position, const glm::vec4& color)
{
	AddCube(position.x, position.y, position.z, color);
}

void Voxel::MeshData::AddCube(unsigned int x, unsigned int y, unsigned int z, const glm::vec4& color)
{
	if (x >= m_Width || y >= m_Height || z >= m_Depth)
	{
		return;
	}

	m_Colors[x][y][z] = Color(color);
}

void Voxel::MeshData::SetData(const std::vector<Cube*>& cubes)
{
	for (Cube* cube : cubes)
	{
		const glm::ivec3& pos = cube->GetPosition();
		if (pos.x < 0 || pos.x >= m_Width || pos.y < 0 || pos.y >= m_Height || pos.z < 0 || pos.z >= m_Depth)
		{
			continue;
		}
		const glm::vec4& color = cube->GetColor();
		m_Colors[pos.x][pos.y][pos.z] = Color(color);
	}
}

Voxel::RenderData* Voxel::MeshData::CalculateRenderData()
{
	if (m_Cache != nullptr)
	{
		return m_Cache;
	}

	glm::ivec3 center = glm::ivec3(m_Width, m_Height, m_Depth) / 2;

	std::vector<Quad> quads;

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			for (int k = 0; k < m_Depth; k++)
			{
				Color color = m_Colors[i][j][k];
				if (color.a == 0.0f)
				{
					continue;
				}

				glm::ivec3 cubePos = glm::ivec3(i, j, k);
				unsigned char flags = GetConnectedBlockFlags(cubePos);

				if (flags == 63)
				{
					continue;
				}

				glm::vec4 cubeCol = glm::vec4(color.r, color.g, color.b, color.a);

				Cube c = Cube(cubePos - center, cubeCol, flags);

				auto cubeQuads = c.GetQuads();
				quads.insert(quads.end(), cubeQuads.begin(), cubeQuads.end());
			}
		}
	}

	size_t verticesSize = quads.size() * 4 * sizeof(Vertex);
	size_t indicesSize = quads.size() * 6 * sizeof(unsigned int);
	float* vertices = (float*)malloc(verticesSize);
	unsigned int* indices = (unsigned int*)malloc(indicesSize);

	int vertexCounter = 0;
	int indexCounter = 0;
	for (int i = 0; i < quads.size(); i++)
	{
		auto data = quads.at(i).GetData();
		memcpy(&vertices[vertexCounter * (sizeof(Vertex) / sizeof(float))], data.vertices, 4 * sizeof(Vertex));
		for (int j = 0; j < 6; j++)
		{
			indices[indexCounter + j] = data.indices[j] + vertexCounter;
		}

		vertexCounter += 4;
		indexCounter += 6;
	}

	m_Cache = new RenderData(vertices, verticesSize, indices, 6 * quads.size());
	return m_Cache;
}

/*
* Since there are multiple constructors for this object.
* This function initialises the mesh data.
*/
void Voxel::MeshData::Init(unsigned int width, unsigned int height, unsigned int depth)
{
	m_Cache = nullptr;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	// Todo: Check if this is needed.
	m_Updated = false;

	m_Colors = new Color * *[m_Width];

	for (int i = 0; i < m_Width; i++)
	{
		m_Colors[i] = new Color * [m_Height];

		for (int j = 0; j < m_Height; j++)
		{
			m_Colors[i][j] = new Color[m_Depth];
		}
	}
}

unsigned char Voxel::MeshData::GetConnectedBlockFlags(const glm::vec3& pos)
{
	unsigned char flags = 0;

	flags |= CheckBlock(pos.x + 1, pos.y, pos.z) << 0;
	flags |= CheckBlock(pos.x - 1, pos.y, pos.z) << 1;
	flags |= CheckBlock(pos.x, pos.y + 1, pos.z) << 2;
	flags |= CheckBlock(pos.x, pos.y - 1, pos.z) << 3;
	flags |= CheckBlock(pos.x, pos.y, pos.z + 1) << 4;
	flags |= CheckBlock(pos.x, pos.y, pos.z - 1) << 5;

	return flags;
}

int Voxel::MeshData::CheckBlock(unsigned int x, unsigned int y, unsigned int z)
{
	if (x >= m_Width || y >= m_Height || z >= m_Depth)
	{
		return 0;
	}

	return m_Colors[x][y][z].a == 1.0f ? 1 : 0;
}

void Voxel::MeshData::DeleteCache()
{
	if (m_Cache)
	{
		free(m_Cache->indices);
		free(m_Cache->vertices);
		delete m_Cache;
	}
}
