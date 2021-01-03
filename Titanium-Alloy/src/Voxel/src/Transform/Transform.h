#pragma once

#include "glm/glm.hpp"

namespace Voxel
{
	/*
	* Keeps track of the translation, scale and rotation of an object.
	*/
	class Transform
	{
	private:
		glm::vec3 m_Translation, m_Scale, m_Rotation;

		/*
		* Cache for the previously calculated transformation / model matrix.
		*/
		glm::mat4 m_Cache;
		bool m_Updated;
	public:
		Transform();
		~Transform();

		/*
		* Setters
		*/
		inline void SetTranslation(const glm::vec3& translation) { m_Translation = translation; m_Updated = true; }
		inline void Translate(const glm::vec3& translation) { m_Translation += translation; m_Updated = true; }

		inline void SetScale(float scale) {	SetScale(glm::vec3(scale)); m_Updated = true; }
		inline void SetScale(const glm::vec3& scale) { m_Scale = scale; m_Updated = true; }

		inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; m_Updated = true; }		// Rotation is radians!
		inline void Rotate(const glm::vec3& rotation) { m_Rotation += rotation; m_Updated = true; }			// Rotation is radians!

		/*
		* Getters
		*/
		inline glm::vec3 GetTranslation() const { return m_Translation; }
		inline glm::vec3 GetScale() const { return m_Scale; }
		inline glm::vec3 GetRotation() const { return m_Rotation; }											// Rotation is radians!

		/*
		* Logic
		*/
		Transform Interpolate(const Transform& t, float percentage);
		Transform Interpolate(Transform* t, float percentage);
		glm::mat4 CalculateModelMatrix();

		// Todo: [ glm::vec3 CalculateNormal() ]
	};
}