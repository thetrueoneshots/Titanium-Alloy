#include "Renderer.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

Renderer::Renderer(Camera* camera)
	: m_ChunkShader(nullptr), m_Camera(camera), m_VP(glm::mat4(1.0f))
{ }

void Renderer::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	if (!m_ChunkShader)
	{
		m_ChunkShader = new Shader("res/shaders/Chunk.shader");
	}
}

void Renderer::Update()
{
	m_VP = m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const
{
	va.Bind();
	ib.Bind();
	s.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawChunk(const Mesh& mesh)
{
	DrawMesh(mesh, *m_ChunkShader);
}

// Todo: Create Renderer::m_VoxelShader for all voxel meshes.
void Renderer::DrawMesh(const Mesh& mesh, Shader& s) const
{
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(3); //Position
	layout.Push<float>(4); //Color
	layout.Push<float>(3); //Normal

	std::vector<Quad> quads = mesh.GetQuads();
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

	VertexBuffer vb(vertices, verticesSize);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6 * quads.size());

	free(vertices);
	free(indices);

	va.Bind();
	ib.Bind();
	s.Bind();

	glm::mat4 mvp = m_VP * mesh.GetModelMatrix();
	s.SetUniform<glm::mat4>("u_MVP", mvp);

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
