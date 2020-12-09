#pragma once

// AnimatedActor: 애니메이션 동작을 가진 Actor 클래스로, Sprite Sheet와 애니메이션 정보를 담은 각각의 json파일이 요구됩니다.

namespace DX
{
	class AnimatedActor :
		public Actor
	{
	public:
		AnimatedActor(const LPCWSTR textureName, const LPCWSTR sheetName, const LPCWSTR animationName);
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

