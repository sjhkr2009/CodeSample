#include "PCH.h"
#include "TimerUI.h"

using namespace SpiritPang;
using namespace DirectX;
using namespace DX;

TimerUI::TimerUI(ID3D11Device1* pDevice, double startTime, float posX, float posY, std::function<void()> eventOnEnd, double warningTime) :
	Actor(L"Assets/UI.png", L"Assets/UI.json"),
	m_setTime{ startTime },
	m_remainTime{ startTime },
	m_pause{ true },
	m_onTimerEnd{ eventOnEnd },
	m_warningTime{ warningTime }
{
	m_text = ActorManager::Instance().Create<Text>(
		static_cast<int>(GameBase::Layer::UIText),
		pDevice,
		L"maplestory.spritefont",
		36.f,
		Colors::Black
	);
	if (warningTime < 0.0)
	{
		warningTime = startTime * 0.2;
	}

	SetFrame(L"remain_time.png");
	SetScale(0.7f);
	SetPosition(posX, posY);

	m_text->SetFontSIze(32.f);
	UpdateUI();
}

void TimerUI::SetPosition(float x, float y)
{
	Actor::SetPosition(x, y);
	m_text->SetPosition(x, y + 35.f);
}

void TimerUI::SetTimer(double startTime, bool pause)
{
	ChangeFontColorToRed(false);
	m_setTime = startTime;
	m_remainTime = startTime;
	m_pause = pause;
	UpdateUI();
}

void TimerUI::ResetTimer()
{
	ChangeFontColorToRed(false);
	m_remainTime = m_setTime;
	m_pause = true;
	UpdateUI();
}

void TimerUI::ChangeFontColorToRed(bool toRed)
{
	if (toRed)
	{
		m_text->SetFontColor(Colors::Red);
	}
	else 
	{
		m_text->SetFontColor(Colors::Black);
	}
}

void TimerUI::Stop(bool reset)
{
	m_pause = true;
	if (reset)
	{
		ResetTimer();
	}
}

void TimerUI::Start(bool reset)
{
	if (reset)
	{
		ResetTimer();
	}
	m_pause = false;
}

void TimerUI::Update(double delta)
{
	if (m_pause)
		return;

	if (SetRemainTime(m_remainTime - delta) == false)
	{
		OnTimerEnd();
	}
	if (m_remainTime < m_warningTime)
	{
		ChangeFontColorToRed();
	}

	UpdateUI();
}

void TimerUI::UpdateUI(int precision)
{
	m_text->SetText(m_remainTime, precision);
}
