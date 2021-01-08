#pragma once
#include "Text.h"

namespace SpiritPang
{
	class ScoreUI : public DX::Actor
	{
	public:
		ScoreUI(ID3D11Device1* pDevice, float posX, float posY);
		~ScoreUI() {};

		void SetScore(int score);
		void OnBlockDestroyByBomb(int count);
		void OnBlockDestroy(int count);
		void OnSwapFailed() { AddScore(-10); }

		void SetPosition(float x, float y);

	private:
		int BlockCountToScore(int count);
		void AddScore(int value) { SetScore(m_score + value); }

	private:
		DX::Text* m_text;

		int m_score;
	};
}
