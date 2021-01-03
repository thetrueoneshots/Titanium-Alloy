#pragma once

#include <glad/GL.h>
#include <assert.h>

#include "../OpenGL/VertexArray.h"
#include "../OpenGL/IndexBuffer.h"
#include "../OpenGL/Shader.h"

#include "../Mesh/Mesh.h"
#include "../Mesh/MeshStore.h"

#include "Camera.h"
#include "RenderContext.h"

namespace Voxel
{
	/* 
	* Rendertypes for built in render contexts
	*/
	enum class RenderType
	{
		VOXEL,
	};

	/*
	* Graphics renderer. Contains multiple functions rendering different types of graphics as well
	* as rendering details, such as view camera
	*/
	class Renderer
	{
	private:
		// Todo: Create camera in renderer and update constructor to get the variables needed
		Camera* m_Camera;
		RenderContext* m_VoxelContext;
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
		* For camera
		* [ void SetView(const glm::vec3& position, const glm::vec3& normal) ]
		* Size is screensize, zoom [1.0f - 45.0f], distance is max distance to render
		* [ void SetProjection(const glm::ivec2& size, float zoom = 45.0f, float distance = 100.0f) ]
		*/
		void Init();
		void Update();

		/*
		* Rendering
		*/
		void Render(Mesh* mesh, RenderContext* context) const;
		void Render(Mesh* mesh, RenderType t = RenderType::VOXEL) const;

		void BatchRender(const std::vector<Transform*>& transforms, Mesh* mesh, RenderContext* context) const;
		void BatchRender(const std::vector<Transform*>& transforms, Mesh* mesh, RenderType t = RenderType::VOXEL) const;

		void AnimatedRender(Mesh* mesh, Animation* animation, RenderContext* context) const;
		void AnimatedRender(Mesh* mesh, Animation* animation, RenderType t = RenderType::VOXEL) const;
	};
}