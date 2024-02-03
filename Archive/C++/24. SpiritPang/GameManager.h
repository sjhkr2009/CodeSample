#pragma once
#include "GameBase.h"
#include "Board.h"
#include "ScoreUI.h"
#include "TimerUI.h"

namespace SpiritPang
{
	class GameManager final : public GameBase
	{
	public:
		~GameManager() override {}

		const float c_spacingLeft{ 100.f };
		const float c_spacingTop{ 100.f };
		const int c_spawnBombPer{ 30 };

		const float c_timeLimit{ 60.f };

	public:
		SpiritPang::Board*			m_board{};
		SpiritPang::CountdownText* m_countdownText{};

		SpiritPang::ScoreUI*		m_scoreUI{};
		SpiritPang::TimerUI*		m_timerUI{};

	private:
		int remainForBombSpawn{ c_spawnBombPer };

	public:
		void Initialize(HWND hwnd, int width, int height) override;

	protected:
		void Update(DX::StepTimer const& timer) override;

	private:
		void OnBlockDestroyed(int count);
		void OnSwapFailed();

		void GameStart();
		void GameOver();

		void OnTimerStop(bool isStop);
	};
}


