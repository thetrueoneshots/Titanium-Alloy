#include "VertexBuffer.h"

#include "../Renderer/Renderer.h"

Voxel::VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

Voxel::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void Voxel::VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void Voxel::VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Voxel::VertexBuffer::SetData(const void* data, unsigned int size) const
{
    unsigned int selectedID;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&selectedID);

    if (selectedID == m_RendererID)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        return;
    }

    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, selectedID);
}
