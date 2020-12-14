#include "RenderContext.h"

Voxel::RenderContext::RenderContext(const std::string& shaderfile, VertexBufferLayout* layout)
{
	m_VA = new VertexArray;
	m_VA->Bind();
	m_Shader = new Shader(shaderfile);
	m_VBL = layout;

	m_VB = new VertexBuffer(nullptr, 0);
	m_VB->Bind();
	m_IB = new IndexBuffer(nullptr, 0);

	m_VA->AddBuffer(*m_VB, *m_VBL);
}

Voxel::RenderContext::~RenderContext()
{
	delete m_VA;
	delete m_Shader;
	delete m_VB;
	delete m_VBL;
	delete m_IB;
}

void Voxel::RenderContext::SetData(RenderData* rData)
{
	SetData(rData->vertices, rData->vertex_array_size, rData->indices, rData->indices_array_count);
}

void Voxel::RenderContext::SetData(float* vData, size_t vSize, unsigned int* iData, size_t iCount)
{
	m_VA->Bind();
	m_VB->SetData(vData, vSize);
	m_IB->SetData(iData, iCount);
}

void Voxel::RenderContext::Bind()
{
	m_VA->Bind();
	m_VB->Bind();
	m_IB->Bind();
	m_Shader->Bind();
}

void Voxel::RenderContext::Render()
{
	Bind();
	glDrawElements(GL_TRIANGLES, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr);
	assert(m_IB->GetCount() > 0);
}
