#include "PCH.h"
#include "Text.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

#ifdef __clang__

#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"

#endif

Text::Text(ID3D11Device1* pDevice, std::wstring spriteFontFilename, float originFontSize, DirectX::XMVECTOR fontColor, TextAlign align) :
	Actor(nullptr),
	m_device{ pDevice },
	m_fontType{ spriteFontFilename },
	m_originFontSize{ originFontSize },
	m_fontColor{ fontColor },
	m_textAlign{ align }
{
	m_font = std::make_unique<SpriteFont>(m_device, m_fontType.c_str());
	SetFontSIze(m_originFontSize);
}

void Text::OnDeviceLost()
{
	m_font.reset();
}

void Text::OnDeviceRestored()
{
	m_font = std::make_unique<SpriteFont>(m_device, L"maplestory.spritefont");
}

void Text::Render(DirectX::SpriteBatch* batch)
{
	const wchar_t* output = m_outputText.c_str();

	Vector2 origin = GetOrigin(output);

	m_font->DrawString(batch, output, GetPosition(), m_fontColor, 0.f, origin, GetScale());
}

void Text::SetText(double text, int precision)
{
	std::wstringstream ss{};
	ss << std::fixed << std::setprecision(precision) << text;
	m_outputText = ss.str();
}

void Text::SetFontSIze(float targetSize)
{
	float size = targetSize / m_originFontSize;
	SetScale(size);
}

Vector2 Text::GetOrigin(const wchar_t* output)
{
	switch (m_textAlign)
	{
		case Text::Center:
			return m_font->MeasureString(output) / 2.f;
		case Text::LeftTop:
			return m_font->MeasureString(output);
		case Text::RightBottom:
			return Vector2(0.f);
		default:
			return Vector2(0.f);
	}
}

float Text::GetContentWidth() const
{
	Vector2 size = m_font->MeasureString(m_outputText.c_str());
	return size.x;
}
