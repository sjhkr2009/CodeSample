#pragma once
#include "Actor.h"

namespace DX
{
	class AnimatedActor :
		public Actor
	{
	public:
		AnimatedActor();
		virtual ~AnimatedActor();

		void LoadAnimation(const LPCWSTR jsonFile);

		virtual void Update(double deltaTime) override;

	public:
		void SetAnimation(const LPCWSTR animName);

		void Play() { m_paused = false; }
		void Pause() { m_paused = true; }
		void Reset() { m_frameIndex = 0; m_totalElapsed = 0.f; }
		void Stop() { Pause(), Reset(); }

		bool IsPaused() { return m_paused; }

	private:
		void SetFrameIndex(int index);

		rapidjson::WDocument*	m_animations;
		rapidjson::WValue*		m_currentAnim;

		bool	m_paused;
		bool	m_isLoop;
		float	m_totalElapsed;
		float	m_timePerFrame;
		int		m_frameIndex;
		int		m_totalFrameCount;
	};
}

