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
	/*
	* Graphics renderer. Contains multiple functions rendering different types of graphics as well
	* as rendering details, such as view camera.
	*/
	class Renderer
	{
	private:
		// Todo: Create camera in renderer and update constructor to get the variables needed
		Camera* m_Camera;
		// Todo: Move shader, vertexarray, vertexbuffer e.t.c. to dedicated struct [ struct RenderEnvironment ]
		Shader* m_ChunkShader;
		VertexArray* m_VertexArray;
		//VertexBufferLayout* m_VertexBufferLayout;
		VertexBuffer* m_VertexBuffer;
		//IndexBuffer* m_IndexBuffer;
		MeshStore* m_Store;
	public:
		/*
		* Constructors and deconstructors
		*/
		Renderer(Camera* camera);
		~Renderer();

		/*
		* Logic
		*/
		/*
		* Todo: Use OpenGL instancing for batch rendering
		* Todo: Rewrite renderer to contain the following functions / enums:
		*
		* [ enum class RenderType {
		* 	VOXEL | RENDER_TYPE_VOXEL,
		* }; ]
		*
		* Returns [ true, is drawn | false, is not drawn (outside camera view / not visible) ]
		* [ bool Render(Mesh* mesh, RenderEnvironment* env | const RenderEnvironment& env) ]
		* Returns [ true, is drawn | false, is not drawn (outside camera view / not visible) ]
		* [ bool Render(Mesh* mesh, RenderType type) ]
		* [ void BatchRender(const std::vector<glm::mat4f>& modelMatrices, Mesh* mesh, RenderType type) ]
		* [ void BatchRender(const std::vector<glm::mat4f>& modelMatrices, Mesh* mesh, RenderEnvironment* env | const RenderEnvironment& env) ]
		*
		* For camera
		* [ void SetView(const glm::vec3& position, const glm::vec3& normal) ]
		* Size is screensize, zoom [1.0f - 45.0f], distance is max distance to render
		* [ void SetProjection(const glm::ivec2& size, float zoom = 45.0f, float distance = 100.0f) ]
		*/
		void Init();
		void Update();
		void Clear() const;

		// Todo: Update according to previous todos
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& s) const;
		void DrawChunk(Mesh* mesh);
		void BatchVoxelDraw(const std::vector<glm::vec3>& positions, Mesh* mesh);
		void BatchVoxelDraw(const std::vector<glm::vec3>& positions, unsigned int key);
		bool AddMesh(unsigned int key, Mesh* m);
		void DrawMesh(Mesh* mesh, Shader* s) const;
	};
}