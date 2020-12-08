#include "Renderer.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

Voxel::Renderer::Renderer(Camera* camera)
	: m_ChunkShader(nullptr), m_Camera(camera)
{ }

void  Voxel::Renderer::Init()
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

void Voxel::Renderer::Update()
{
}

void Voxel::Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const
{
	va.Bind();
	ib.Bind();
	s.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Voxel::Renderer::DrawChunk(Mesh* mesh)
{
	DrawMesh(mesh, m_ChunkShader);
}


void Voxel::Renderer::DrawMesh(Mesh* mesh, Shader* s) const
{
	VertexArray va;
	VertexBufferLayout layout;
	layout.Push<float>(3); //Position
	layout.Push<float>(4); //Color
	layout.Push<float>(3); //Normal

	RenderData* data = mesh->GetRenderData();

	VertexBuffer vb(data->vertices, data->vertex_array_size);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(data->indices, data->indices_array_count);

	va.Bind();
	ib.Bind();
	s->Bind();

	s->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	s->SetUniform("u_View", m_Camera->GetViewMatrix());
	s->SetUniform("u_Model", mesh->GetModelMatrix());

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Voxel::Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
