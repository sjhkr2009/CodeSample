#include "PCH.h"
#include "ScoreUI.h"

using namespace SpiritPang;
using namespace DirectX;
using namespace DX;

ScoreUI::ScoreUI(ID3D11Device1* pDevice, float posX, float posY) :
	Actor(L"Assets/UI.png", L"Assets/UI.json")
{
	m_text = ActorManager::Instance().Create<Text>(
		static_cast<int>(GameBase::Layer::UIText),
		pDevice,
		L"maplestory.spritefont",
		36.f,
		Colors::Black
	);
	SetFrame(L"scoreboard.png");
	SetScore(0);
	SetScale(0.7f);
	SetPosition(posX, posY);
}

void ScoreUI::SetScore(int score)
{
	m_score = std::max(score, 0);
	m_text->SetText(m_score);

	float fontSize = std::min(32.f, 32.f - 0.1f * (m_text->GetContentWidth() - 100.f));
	m_text->SetFontSIze(fontSize);
}

void ScoreUI::OnBlockDestroyByBomb(int count)
{
	AddScore(count * 35);
}

void ScoreUI::OnBlockDestroy(int count)
{
	int score = BlockCountToScore(count);
	
	AddScore(score);
}

void ScoreUI::SetPosition(float x, float y)
{
	Actor::SetPosition(x, y);
	m_text->SetPosition(x, y + 35.f);
}

int ScoreUI::BlockCountToScore(int count)
{
	int bonus = static_cast<int>(std::pow(2, std::max(0, count - 3)));
	if (bonus <= 1)
	{
		bonus = 0;
	}
	int score = 10 * ((count * (count + 1)) / 2);
	score += std::min(bonus, 1000);
	
	return score;
}
