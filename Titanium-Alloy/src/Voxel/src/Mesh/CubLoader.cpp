#include "CubLoader.h"

struct Color
{
	char r, g, b;
};

struct Cub
{
	int width, depth, height;
	Color* blocks; //Size = width * depth * height
};

Voxel::Mesh* Voxel::CubLoader::LoadMeshFromFile(const std::string& file, const std::string& path)
{
	char buffer[250];

	std::string filepath(path.c_str());
	filepath.append(file);

	std::ifstream stream;

	sprintf_s(buffer, 250, "Loading cube file: %s", filepath.c_str());
	std::cout << buffer << std::endl;

	stream.open(filepath, std::ios::in);
	if (!stream.is_open())
	{
		return nullptr;
	}
	stream.seekg(0);
	size_t bytes = 0;

	int w, d, h;
	stream.read((char*)&w, sizeof(w));
	bytes += sizeof(w);
	stream.read((char*)&d, sizeof(d));
	bytes += sizeof(d);
	stream.read((char*)&h, sizeof(h));
	bytes += sizeof(h);

	sprintf_s(buffer, 250, "Cube size: %d, %d, %d", w, h, d);
	std::cout << buffer << std::endl;

	Mesh* m = new Mesh(w, h, d);

	int counter = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < d; j++)
		{
			for (int k = 0; k < w; k++)
			{
				unsigned char r, g, b;

				stream.read((char*)&r, sizeof(r));
				stream.read((char*)&g, sizeof(g));
				stream.read((char*)&b, sizeof(b));

				bytes += sizeof(r) + sizeof(g) + sizeof(b);

				if (r == 0 && g == 0 && b == 0)
				{
					continue;
				}

				counter++;

				glm::vec4 color = glm::vec4(r, g, b, 255.0f) / 255.0f;
				glm::ivec3 pos = glm::ivec3(k, i, j);
				m->AddCube(pos, color);
			}
		}
	}

	std::cout << "Blocks added to the mesh: " << counter << std::endl;

	sprintf_s(buffer, 250, "Amount of bytes read: %d", bytes);
	std::cout << buffer << std::endl;
	stream.close();

	return m;
}
