#pragma once
#include "GameBase.h"
#include "Board.h"
#include "ScoreText.h"

namespace SpiritPang
{
	class GameManager final : public GameBase
	{
	public:
		~GameManager() override {}

		const float spacingLeft{ 100.f };
		const float spacingTop{ 100.f };
		const int spawnBombPer{ 30 };

	public:
		SpiritPang::Board* m_board{};
		SpiritPang::ScoreText* m_scoreText{};

	private:
		int remainForBombSpawn{ spawnBombPer };

	public:
		void Initialize(HWND hwnd, int width, int height) override;

	protected:
		void Update(DX::StepTimer const& timer) override;

	private:
		void OnBlockDestroyed(int count);
		void OnSwapFailed();

	};
}


