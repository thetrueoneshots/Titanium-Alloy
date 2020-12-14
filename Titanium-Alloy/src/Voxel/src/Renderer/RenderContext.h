#pragma once

#include "../OpenGL/VertexArray.h"
#include "../OpenGL/IndexBuffer.h"
#include "../OpenGL/Shader.h"

#include "../Mesh/DataTypes.h"

namespace Voxel
{
	class RenderContext
	{
	private:
		VertexArray* m_VA;
		Shader* m_Shader;
		VertexBuffer* m_VB;
		VertexBufferLayout* m_VBL;
		IndexBuffer* m_IB;
	public:
		RenderContext(const std::string& shaderfile, VertexBufferLayout* layout);
		~RenderContext();

		void SetData(RenderData* renderData);
		void SetData(float* vData, size_t vSize, unsigned int* iData, size_t iCount);
		void Bind();

		inline Shader* GetShader() { m_Shader->Bind();  return m_Shader; }
		void Render();
	};
}