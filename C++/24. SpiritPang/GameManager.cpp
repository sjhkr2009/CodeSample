#include "PCH.h"
#include "GameManager.h"

using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)

void GameManager::Initialize(HWND hwnd, int width, int height)
{
	GameBase::Initialize(hwnd, width, height);

	m_board = ActorManager::Instance().Create<Board>(static_cast<int>(Layer::Background), c_spacingLeft, c_spacingTop);
	m_scoreUI = ActorManager::Instance().Create<ScoreUI>(
		static_cast<int>(Layer::UI),
		m_deviceResources->GetD3DDevice(),
		c_spacingLeft + m_board->GetCurrentSize().x + 100.f,
		c_spacingTop + 100.f
	);
	m_timerUI = ActorManager::Instance().Create<TimerUI>(
		static_cast<int>(Layer::UI),
		m_deviceResources->GetD3DDevice(),
		c_timeLimit,
		c_spacingLeft + m_board->GetCurrentSize().x + 100.f,
		c_spacingTop + 300.f,
		[&]() { GameOver(); },
		10.0
		);
	m_countdownText = ActorManager::Instance().Create<CountdownText>(static_cast<int>(Layer::UIText), [&]() { GameStart(); });

	m_board->SetEventOnBlockDestroy([&](int count) { OnBlockDestroyed(count); });
	m_board->SetEventOnSwapFailed([&]() { OnSwapFailed(); });
	m_board->SetEventOnTimerStop([&](bool stop) { OnTimerStop(stop);});

	m_countdownText->SetPosition(
		c_spacingLeft + m_board->GetCurrentSize().x * 0.5f,
		c_spacingTop + m_board->GetCurrentSize().y * 0.5f
	);
	m_countdownText->Play();
}

void GameManager::Update(DX::StepTimer const& timer)
{
	GameBase::Update(timer);
}

void GameManager::OnBlockDestroyed(int count)
{
	int state = m_board->GetState();
	if (state & Board::State::Exploding)
	{
		m_scoreUI->OnBlockDestroyByBomb(count);
	}
	else
	{
		m_scoreUI->OnBlockDestroy(count);
		remainForBombSpawn -= count;
		if (remainForBombSpawn <= 0)
		{
			m_board->BombSpawn();
			remainForBombSpawn = c_spawnBombPer;
		}
	}
}

void GameManager::OnSwapFailed()
{
	m_scoreUI->OnSwapFailed();
}

void GameManager::GameStart()
{
	m_board->Resume();
	m_timerUI->Start(true);
}

void GameManager::GameOver()
{
	m_board->Pause();
}

void GameManager::OnTimerStop(bool isStop)
{
	if (isStop)
	{
		m_timerUI->Stop();
	}
	else
	{
		m_timerUI->Start();
	}
}

#pragma warning(pop)
