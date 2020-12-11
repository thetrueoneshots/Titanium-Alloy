#pragma once

#include <glad/GL.h>
#include <assert.h>

#include "../OpenGL/VertexArray.h"
#include "../OpenGL/IndexBuffer.h"
#include "../OpenGL/Shader.h"

#include "../Mesh/Mesh.h"
#include "../Mesh/MeshStore.h"

#include "Camera.h"

namespace Voxel
{
	class Renderer
	{
	private:
		Camera* m_Camera;
		Shader* m_ChunkShader;
		VertexArray* m_VertexArray;
		//VertexBufferLayout* m_VertexBufferLayout;
		VertexBuffer* m_VertexBuffer;
		//IndexBuffer* m_IndexBuffer;
		MeshStore* m_Store;
	public:
		Renderer(Camera* camera);
		~Renderer();

		void Init();
		void Update();
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const;
		// Shader is non-const, because of setting mvp. Standard shader should be made a member;
		void DrawChunk(Mesh* mesh);
		void BatchVoxelDraw(const std::vector<glm::vec3>& positions, Mesh* mesh);
		void BatchVoxelDraw(const std::vector<glm::vec3>& positions, unsigned int key);
		bool AddMesh(unsigned int key, Mesh* m);
		void DrawMesh(Mesh* mesh, Shader* s) const;
		void Clear() const;
	};
}