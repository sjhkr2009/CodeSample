#pragma once
#include "Actor.h"

namespace SpiritPang
{
	class ScoreText : public DX::Actor
	{
	public:
		ScoreText(ID3D11Device1* pDevice);

		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		void Render(DirectX::SpriteBatch* batch) override;

		void SetScore(int score) { m_score = std::max(score, 0); m_outputText = std::to_wstring(m_score); }
		void OnBlockDestroyByBomb(int count);
		void OnBlockDestroy(int count);
		void OnSwapFailed() { AddScore(-10); }

	private:
		int BlockCountToScore(int count);
		void AddScore(int value) { SetScore(m_score + value); }

	private:
		std::unique_ptr<DirectX::SpriteFont> m_font;
		ID3D11Device1* m_device;

		int m_score;
		std::wstring m_outputText;
	};
}
