#include "Quad.h"

QuadData Quad::GetData() const
{
	//float* vertices = (float*)_malloca(4 * sizeof(Vertex));

	Vertex vertices[] = {
		{m_P1, m_Color, m_Normal},
		{m_P2, m_Color, m_Normal},
		{m_P3, m_Color, m_Normal},
		{m_P4, m_Color, m_Normal},
		//m_P1.x, m_P1.y, m_P1.z, m_Color.r, m_Color.g, m_Color.b, m_Color.a,
		//m_P2.x, m_P2.y, m_P2.z, m_Color.r, m_Color.g, m_Color.b, m_Color.a,
		//m_P3.x, m_P3.y, m_P3.z, m_Color.r, m_Color.g, m_Color.b, m_Color.a,
		//m_P4.x, m_P4.y, m_P4.z, m_Color.r, m_Color.g, m_Color.b, m_Color.a,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	return { vertices, indices };
}