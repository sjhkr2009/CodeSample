#pragma once
#include "Actor.h"
#include <string>
#include <iomanip>
#include <sstream>

namespace DX
{
	class Text : public Actor
	{
	public:
		enum TextAlign
		{
			Center = 1,
			LeftTop = 2,
			RightBottom = 3
		};

	public:
		Text(ID3D11Device1* pDevice,
			std::wstring spriteFontFilename,
			float originFontSize,
			DirectX::XMVECTOR fontColor,
			TextAlign align = TextAlign::Center
		);
		virtual ~Text() {};

		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		virtual void Render(DirectX::SpriteBatch* batch) override;

		template<typename T>
		void SetText(T text) { m_outputText = std::to_wstring(text); }
		void SetText(std::string text) { m_outputText.assign(text.begin(), text.end()); }
		void SetText(double text, int precision);

#pragma warning(push)
#pragma warning(disable:26812)
		void SetAlign(TextAlign alignMode) { m_textAlign = alignMode; }
#pragma warning(pop)
		void SetFontSIze(float targetSize);

		DirectX::SimpleMath::Vector2 GetOrigin(const wchar_t* output);
		float GetContentWidth() const;

		float GetFontSize() const { return m_originFontSize * GetScale(); }
		void SetFontColor(DirectX::XMVECTOR color) { m_fontColor = color; }

	public:
		float m_fontSize{ 36.f };

	private:
		std::unique_ptr<DirectX::SpriteFont> m_font;
		ID3D11Device1* m_device;

		std::wstring		m_fontType;
		DirectX::XMVECTOR	m_fontColor;
		TextAlign			m_textAlign;
		float				m_originFontSize;

		std::wstring		m_outputText{};
	};
}

