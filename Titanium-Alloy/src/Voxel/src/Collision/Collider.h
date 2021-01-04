#pragma once

#include "Box.h"
#include "Line.h"

namespace Voxel
{
	class Mesh;

	class Collider
	{
	public:

		template <typename T1, typename T2>
		static bool Collision(T1 t1, T2 t2);
	private:
		Collider() {}
	};

	template <typename T1, typename T2>
	inline bool Collider::Collision(T1 t1, T2 t2)
	{
		return false;
	}

	template <>
	inline bool Collider::Collision(Box t1, Box t2)
	{
		return t1.CollidesWith(t2);
	}

	template <>
	inline bool Collider::Collision(Mesh t1, Box t2)
	{
		return t2.CollidesWith(t1);
	}

	template <>
	inline bool Collider::Collision(Box t1, Mesh t2)
	{
		return t1.CollidesWith(t2);
	}

	template <>
	inline bool Collider::Collision(Line t1, Box t2)
	{
		return t1.CollidesWith(t2);
	}

	template <>
	inline bool Collider::Collision(Box t1, Line t2)
	{
		return t2.CollidesWith(t1);
	}

}