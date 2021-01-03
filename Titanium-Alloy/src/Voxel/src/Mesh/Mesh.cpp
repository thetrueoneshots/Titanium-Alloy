#include "Mesh.h"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

Voxel::Mesh::Mesh(const glm::ivec3& size, Transform* transform)
{
	m_SharedData = false;
	m_MeshData = new MeshData(size.x, size.y, size.z);

	m_Transform = transform;

	m_Animation = nullptr;
}

Voxel::Mesh::Mesh(unsigned int w, unsigned int h, unsigned int d, Transform* transform)
{
	m_SharedData = false;
	m_MeshData = new MeshData(w, h, d);

	m_Transform = transform;

	m_Animation = nullptr;
}

Voxel::Mesh::Mesh(MeshData* data, Transform* transform)
{
	if (data != nullptr)
	{
		m_SharedData = true;
		m_MeshData = data;
	}
	else
	{
		m_SharedData = false;
		m_MeshData = new MeshData(0, 0, 0);
	}

	if (m_Transform != nullptr)
	{
		m_Transform = transform;
	}
	else
	{
		m_Transform = new Transform;
	}

	m_Animation = nullptr;
}

Voxel::Mesh::~Mesh()
{
	delete m_Transform;
	delete m_MeshData;
}

void Voxel::Mesh::ScaleToSize(float scale, bool keepRatio)
{
	ScaleToSize(glm::ivec3(scale), keepRatio);
}

void Voxel::Mesh::ScaleToSize(const glm::vec3& scale, bool keepRatio)
{
	if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
	{
		return;
	}

	glm::ivec3 size = m_MeshData->GetSize();

	glm::vec3 calcScale;
	if (keepRatio)
	{
		unsigned int max = (size.x > size.z) ? size.x : size.z;
		calcScale = glm::vec3(scale.x / max, scale.y / max, scale.z / max);
	}
	else
	{
		calcScale = glm::vec3(scale.x / size.x, scale.y / size.y, scale.z / size.z);
	}
	m_Transform->SetScale(calcScale);
}

void Voxel::Mesh::ScaleXToSize(float scale)
{
	glm::ivec3 size = m_MeshData->GetSize();
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.x *= scale / size.x;
	m_Transform->SetScale(current);
}

void Voxel::Mesh::ScaleYToSize(float scale)
{
	glm::ivec3 size = m_MeshData->GetSize();
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.y *= scale / size.y;
	m_Transform->SetScale(current);
}

void Voxel::Mesh::ScaleZToSize(float scale)
{
	glm::ivec3 size = m_MeshData->GetSize();
	if (scale <= 0)
	{
		return;
	}

	glm::vec3 current = m_Transform->GetScale();
	current.z *= scale / size.z;
	m_Transform->SetScale(current);
}