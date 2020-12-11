#pragma once

namespace Voxel
{
	class VertexBuffer
	{
	private:
		unsigned int m_RendererID;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, unsigned int size) const;
	};
}