#pragma once

#include <vector>

#include "../Transform/Transform.h"

namespace Voxel
{
	struct AnimationFrame
	{
		double time;
		Transform t;
	};

	class Animation
	{
	private:
		std::vector<AnimationFrame> m_Frames;
		double m_Time;
		double m_MaxTime;
		Transform m_Original;

		bool m_Active;
		int m_Index;
	public:
		Animation(Transform original, double maxTime);
		~Animation();

		void InsertFrame(const AnimationFrame& frame);

		Transform GetTransform(double timeElapsed);
		void Play();
		void Pause();
		void Reset();
	private:
	};
}