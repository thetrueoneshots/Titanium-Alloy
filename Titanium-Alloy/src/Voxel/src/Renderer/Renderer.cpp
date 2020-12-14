#include "Renderer.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

Voxel::Renderer::Renderer(Camera* camera)
	: m_ChunkShader(nullptr), m_Camera(camera), m_VertexArray(nullptr), m_VertexBuffer(nullptr)
{ }

Voxel::Renderer::~Renderer(){
	if (m_ChunkShader)
	{
		delete m_ChunkShader;
	}
	if (m_VertexArray)
	{
		delete m_VertexArray;
	}
	if (m_VertexBuffer)
	{
		delete m_VertexBuffer;
	}
}

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

	m_VertexArray = new VertexArray;
	VertexBufferLayout layout;
	layout.Push<float>(1); //Position
	layout.Push<float>(1); //Color
	layout.Push<float>(1); //Normal

	m_VertexBuffer = new VertexBuffer(nullptr, 0);
	m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
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

void Voxel::Renderer::BatchVoxelDraw(const std::vector<glm::vec3>& positions, Mesh* mesh)
{
	RenderData* data = mesh->GetRenderData();

	m_VertexArray->Bind();
	m_VertexBuffer->SetData(data->vertices, data->vertex_array_size);

	IndexBuffer ib(data->indices, data->indices_array_count);
	ib.Bind();
	m_ChunkShader->Bind();
	m_ChunkShader->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	m_ChunkShader->SetUniform("u_View", m_Camera->GetViewMatrix());

	glm::vec3 oldPosition = mesh->GetTransForm()->GetTranslation();

	// Todo: Calculate MVP once per frame
	glm::vec3 prev = glm::vec3(0);
	for (const auto& position : positions)
	{
		mesh->GetTransForm()->Translate(position - prev);
		prev = position;
		m_ChunkShader->SetUniform("u_Model", mesh->GetTransForm()->CalculateModelMatrix());
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	mesh->GetTransForm()->SetTranslation(oldPosition);
}

void Voxel::Renderer::BatchVoxelDraw(const std::vector<Transform*>& transforms, Mesh* mesh)
{
	RenderData* data = mesh->GetRenderData();

	m_VertexArray->Bind();
	m_VertexBuffer->SetData(data->vertices, data->vertex_array_size);

	IndexBuffer ib(data->indices, data->indices_array_count);
	ib.Bind();
	m_ChunkShader->Bind();
	m_ChunkShader->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	m_ChunkShader->SetUniform("u_View", m_Camera->GetViewMatrix());

	// Todo: Calculate MVP once per frame
	Transform t = *mesh->GetTransForm();

	for (const auto& transform : transforms)
	{
		Transform temp = Transform(t);
		temp.Translate(transform->GetTranslation());
		temp.Rotate(transform->GetRotation());
		temp.SetScale(temp.GetScale() * transform->GetScale());
		m_ChunkShader->SetUniform("u_Model", temp.CalculateModelMatrix());
		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}


void Voxel::Renderer::DrawMesh(Mesh* mesh, Shader* s) const
{
	RenderData* data = mesh->GetRenderData();

	m_VertexArray->Bind();
	m_VertexBuffer->SetData(data->vertices, data->vertex_array_size);

	IndexBuffer ib(data->indices, data->indices_array_count);
	ib.Bind();
	s->Bind();

	// Todo: Calculate MVP once per frame
	s->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	s->SetUniform("u_View", m_Camera->GetViewMatrix());
	s->SetUniform("u_Model", mesh->GetTransForm()->CalculateModelMatrix());

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Voxel::Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
