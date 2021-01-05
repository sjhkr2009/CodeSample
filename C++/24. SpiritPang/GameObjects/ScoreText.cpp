#include "PCH.h"
#include "ScoreText.h"

using namespace SpiritPang;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

ScoreText::ScoreText(ID3D11Device1* pDevice) :
	Actor(nullptr),
	m_device{ pDevice },
	m_score{},
	m_outputText{ std::to_wstring(m_score) }
{
	m_font = std::make_unique<SpriteFont>(m_device, L"maplestory.spritefont");

	SetPosition(200.f, 50.f);
}

void ScoreText::OnDeviceLost()
{
	m_font.reset();
}

void ScoreText::OnDeviceRestored()
{
	m_font = std::make_unique<SpriteFont>(m_device, L"maplestory.spritefont");
}

void ScoreText::Render(SpriteBatch* batch)
{
	const wchar_t* score = m_outputText.c_str();

	Vector2 origin = m_font->MeasureString(score) / 2.f;

	m_font->DrawString(batch, score, GetPosition(), Colors::White, 0.f, origin);
}

void ScoreText::OnBlockDestroyByBomb(int count)
{
	AddScore(count * 35);
}

void ScoreText::OnBlockDestroy(int count)
{
	int score = BlockCountToScore(count);
	
	AddScore(score);
}

int ScoreText::BlockCountToScore(int count)
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
