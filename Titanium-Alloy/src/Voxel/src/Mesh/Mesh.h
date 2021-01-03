#pragma once

// Todo: Check all needed includes
#include <vector>
#include <map>
#include <utility>

#include "glm/glm.hpp"

#include "MeshData.h"

#include "../Renderer/Transform.h"
#include "../Animation/Animation.h"


namespace Voxel
{
	/*
	* A mesh containing all cubes, as well as model state, such as rotation, translation and scale
	*/
	class Mesh
	{
	private:
		// Todo move width, height, cubes and updated to MeshData
		MeshData* m_MeshData;
		bool m_SharedData;
		Transform* m_Transform;
		Animation* m_Animation;
	public:
		/*
		* Constructors and deconstructor
		* Todo: Constructor with MeshData
		*/
		Mesh(const glm::ivec3& size, Transform* transform = new Transform);
		Mesh(unsigned int w, unsigned int h, unsigned int d, Transform* transform = new Transform);
		Mesh(MeshData* data, Transform* transform = new Transform);
		~Mesh();

		/*
		* Getters
		*/
		inline Transform* GetTransForm() const { return m_Transform; }
		inline MeshData* GetMeshData() const { return m_MeshData; }
		inline Animation* GetAnimation() const { return m_Animation; }

		/*
		* Setters
		*/
		inline void SetTransform(Transform* transform, bool cleanup = true) { if (cleanup) delete m_Transform; m_Transform = transform; }
		inline void SetMeshData(MeshData* data, bool cleanup = true) { if (cleanup) delete m_MeshData; m_MeshData = data; }
		// The extra check for the existence of m_Animation is because not every mesh has an animation.
		inline void SetAnimation(Animation* animation, bool cleanup = true) { if (cleanup && m_Animation) delete m_Animation; m_Animation = animation; }

		/*
		* Scaling the mesh to a certain block size
		* Note: With keepRatio = true, the max of x and z are considered for the scale
		* Todo: Move to transform
		*/
		void ScaleToSize(float scale, bool keepRatio = true);
		void ScaleToSize(const glm::vec3& scale, bool keepRatio = true);

		/*
		* Scaling every axis to a certain size for only scaling one axis
		* Todo: Move to transform
		*/
		void ScaleXToSize(float scale);
		void ScaleYToSize(float scale);
		void ScaleZToSize(float scale);
	};
}