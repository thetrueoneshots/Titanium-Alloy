#pragma once

#include <glad/GL.h>
#include <assert.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Camera.h"

#include "Geometry/Mesh.h"

const static float FIELD_OF_VIEW = 45.0f;

static 

class Renderer
{
private:
	Camera m_Camera;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_VP;
	Shader* m_ChunkShader;
public:
	Renderer(int WindowWidth, int WindowHeight);
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
	void DrawChunk(const Mesh& mesh);
	void DrawMesh(const Mesh& mesh, Shader& s) const;
	void Clear() const;

	inline Camera* GetCamera() { return &m_Camera; }
};