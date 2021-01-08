#pragma once

// AnimatedActor: 애니메이션 동작을 가진 Actor 클래스로, Sprite Sheet와 애니메이션 정보를 담은 각각의 json파일이 요구됩니다.

namespace DX
{
#pragma warning(push)
#pragma warning(disable:26812)
	class AnimatedActor :
		public Actor
	{
	public:
		enum OnEnd
		{
			None = 0,
			Disable = 1,
			Loop = 2
		};

	public:
		AnimatedActor(const LPCWSTR textureName, const LPCWSTR sheetName, const LPCWSTR animationName);
		virtual ~AnimatedActor();

		void LoadAnimation(const LPCWSTR jsonFile);

		virtual void Update(double deltaTime) override;

	public:
		void SetAnimation(const LPCWSTR animName);
		void SetOnEnd(OnEnd action) { m_onEnd = action; }

		virtual void Play() { SetActive(true), Reset(); m_paused = false; KeyFrameAnimation(m_frameIndex); }
		void Pause() { m_paused = true; }
		void Resume() { m_paused = false; }
		void Reset() { m_frameIndex = 0; m_totalElapsed = 0.f; }
		void Stop() { Pause(), Reset(); }

		bool IsPaused() { return m_paused; }

	protected:
		virtual void KeyFrameAnimation(int nowIndex) {};
		virtual void OnAnimationEnd() {};

		int		m_totalFrameCount;
		int		m_frameIndex;

	private:
		void SetFrameIndex(int index);

		rapidjson::WDocument*	m_animations;
		rapidjson::WValue*		m_currentAnim;

		OnEnd	m_onEnd;

		bool	m_paused;
		float	m_totalElapsed;
		float	m_timePerFrame;
	};
#pragma warning(pop)
}

