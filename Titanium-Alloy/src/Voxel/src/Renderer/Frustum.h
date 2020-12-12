#pragma once
// Todo: Cleanup + implement
#include "glm/glm.hpp"

struct Plane
{
	glm::vec3 normal;
	float distance;
};

namespace Voxel
{
	class CameraFrustum
	{
	private:

		float* m_RenderDistance;
		float* m_Fov;
		glm::ivec2* m_ScreenSize;
		glm::vec3* m_Normal;
		glm::vec3* m_Position;

		Plane m_Planes[6];
		//glm::vec3 m_NormalArray[6];
		//float m_DArray[6];

	public:
		CameraFrustum(glm::ivec2* screenSize, glm::vec3* normal, glm::vec3* position, float* fov, float* distance);
		~CameraFrustum();

		void Update(const glm::mat4& vp);

		bool IsInFrustum(const glm::vec3& pos, float radius);
	};
}