#include "Renderer.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

Voxel::Renderer::Renderer(Camera* camera)
	: m_VoxelContext(nullptr), m_Camera(camera)
{ }

Voxel::Renderer::~Renderer() {
	if (m_VoxelContext != nullptr)
	{
		delete m_VoxelContext;
	}
}

void Voxel::Renderer::Init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_MULTISAMPLE);

	if (!m_VoxelContext)
	{
		VertexBufferLayout* layout = new VertexBufferLayout;
		layout->Push<float>(1); //Position
		layout->Push<float>(1); //Color
		layout->Push<float>(1); //Normal
		m_VoxelContext = new RenderContext("res/shaders/Chunk.shader", layout);
	}
}

void Voxel::Renderer::Update()
{

}

void Voxel::Renderer::Render(Mesh* mesh, RenderContext* context) const
{
	RenderData* data = mesh->GetRenderData();
	context->SetData(data);

	Shader* s = context->GetShader();

	s->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	s->SetUniform("u_View", m_Camera->GetViewMatrix());
	s->SetUniform("u_Model", mesh->GetTransForm()->CalculateModelMatrix());

	context->Render();
}

void Voxel::Renderer::Render(Mesh* mesh, RenderType t) const
{
	switch (t)
	{
	case RenderType::VOXEL:
		Render(mesh, m_VoxelContext);
		break;
	default:
		Render(mesh, m_VoxelContext);
		break;
	}
}

void Voxel::Renderer::BatchRender(const std::vector<Transform*>& transforms, Mesh* mesh, RenderContext* context) const
{
	RenderData* data = mesh->GetRenderData();
	context->SetData(data);

	Shader* s = context->GetShader();

	s->SetUniform("u_Projection", m_Camera->GetProjectionMatrix());
	s->SetUniform("u_View", m_Camera->GetViewMatrix());

	Transform t = *mesh->GetTransForm();

	for (const auto& transform : transforms)
	{
		Transform temp = Transform(t);
		temp.Translate(transform->GetTranslation());
		temp.Rotate(transform->GetRotation());
		temp.SetScale(temp.GetScale() * transform->GetScale());

		s->SetUniform("u_Model", temp.CalculateModelMatrix());

		context->Render();
	}
}

void Voxel::Renderer::BatchRender(const std::vector<Transform*>& transforms, Mesh* mesh, RenderType t) const
{
	switch (t)
	{
	case RenderType::VOXEL:
		BatchRender(transforms, mesh, m_VoxelContext);
		break;
	default:
		BatchRender(transforms, mesh, m_VoxelContext);
		break;
	}
}
