#pragma once
#include "Text.h"

namespace SpiritPang 
{
	class TimerUI : public DX::Actor
	{
	public:
		TimerUI(ID3D11Device1* pDevice, double startTime, float posX, float posY, std::function<void()> eventOnEnd, double warningTime = -1.0);
		~TimerUI() {};

		void SetPosition(float x, float y);
		void SetTimer(double startTime, bool pause = true);
		void ResetTimer();

		void ChangeFontColorToRed(bool toRed = true);

		void Stop(bool reset = false);
		void Start(bool reset = false);

		virtual void Update(double delta);

	private:
		bool SetRemainTime(double time) { m_remainTime = (time > 0.0) ? time : 0.0; return (m_remainTime > 0.0); }
		void OnTimerEnd() { m_pause = true; m_onTimerEnd(); }
		void UpdateUI(int precision = 1);

	private:
		DX::Text* m_text;

		double m_setTime;
		double m_remainTime;
		double m_warningTime;
		bool m_pause;

		std::function<void()> m_onTimerEnd;
	};
}

