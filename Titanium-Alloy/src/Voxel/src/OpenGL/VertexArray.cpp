#include "VertexArray.h"

Voxel::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

Voxel::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void Voxel::VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void Voxel::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Voxel::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements.at(i);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride() , (const void*)offset);

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
