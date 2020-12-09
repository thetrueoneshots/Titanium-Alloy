#pragma once

#include <glad/GL.h>
#include <assert.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"

#include "Mesh.h"

namespace Voxel
{
	class Renderer
	{
	private:
		Camera* m_Camera;
		Shader* m_ChunkShader;
		VertexArray* m_VertexArray;
		VertexBufferLayout* m_VertexBufferLayout;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
	public:
		Renderer(Camera* camera);
		~Renderer() {
			if (m_ChunkShader)
			{
				delete m_ChunkShader;
			}
		}
		void Init();
		void Update();
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const;
		// Shader is non-const, because of setting mvp. Standard shader should be made a member;
		void DrawChunk(Mesh* mesh);
		void BatchVoxelDraw(const std::vector<glm::vec3>& positions, Mesh* mesh);
		void DrawMesh(Mesh* mesh, Shader* s) const;
		void Clear() const;
	};
}