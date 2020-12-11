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

	sprintf_s(buffer, 250, "Cube size: %d, %d, %d", w, d, h);
	std::cout << buffer << std::endl;

	std::vector<Voxel::Cube*> cubes;

	int counter = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			for (int k = 0; k < d; k++)
			{
				unsigned char r, g, b;
				Color c;
				stream.read((char*)&c.r, sizeof(c.r));
				stream.read((char*)&c.g, sizeof(c.g));
				stream.read((char*)&c.b, sizeof(c.b));

				bytes += sizeof(c.r) + sizeof(c.g) + sizeof(c.b);
				r = c.r;
				g = c.g;
				b = c.b;
				
				counter++;

				if (r == 0 && g == 0 && b == 0)
				{
					continue;
				}

				glm::vec4 color = glm::vec4(r, g, b, 255.0f) / 255.0f;
				glm::vec3 pos = glm::vec3(k, i, j);
				cubes.push_back(new Cube(pos, color));

			}
		}
	}
	sprintf_s(buffer, 250, "Amount of bytes read: %d", bytes);
	std::cout << buffer << std::endl;
	stream.close();

	Mesh* m = new Mesh(cubes);
	if (m == nullptr)
	{
		for (auto& item : cubes)
		{
			delete item;
		}
		return nullptr;
	}
	m->UpdateRenderFlags();
	return m;
}
