#include "PCH.h"
#include "CountdownText.h"

using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)
CountdownText::CountdownText(std::function<void()> evt) :
	AnimatedActor(L"Assets/UI.png", L"Assets/UI.json", L"Assets/AnimCountdown.json"),
	m_onDone{ evt }
{
	SetOnEnd(AnimatedActor::OnEnd::Disable);
	SetAnimation(L"countdown");
}
#pragma warning(pop)

void CountdownText::Update(double deltaTime)
{
	AnimatedActor::Update(deltaTime);

	if (m_frameIndex < (m_totalFrameCount - 1))
	{
		SetScale(GetScale() * 0.9f);
	}
}

void CountdownText::KeyFrameAnimation(int nowIndex)
{
	SetScale(1.0f);
}

void CountdownText::OnAnimationEnd()
{
	m_onDone();
}
