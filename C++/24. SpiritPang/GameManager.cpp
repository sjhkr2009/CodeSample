#include "PCH.h"
#include "GameManager.h"

using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)

void GameManager::Initialize(HWND hwnd, int width, int height)
{
	GameBase::Initialize(hwnd, width, height);

	m_board = ActorManager::Instance().Create<Board>(static_cast<int>(Layer::Background), spacingLeft, spacingTop);
	m_scoreText = ActorManager::Instance().Create<ScoreText>(static_cast<int>(Layer::UI), m_deviceResources->GetD3DDevice());

	m_board->SetEventOnBlockDestroy([&](int count) { OnBlockDestroyed(count); });
	m_board->SetEventOnSwapFailed([&]() { OnSwapFailed(); });
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
		m_scoreText->OnBlockDestroyByBomb(count);
	}
	else
	{
		m_scoreText->OnBlockDestroy(count);
		remainForBombSpawn -= count;
		if (remainForBombSpawn <= 0)
		{
			m_board->BombSpawn();
			remainForBombSpawn = spawnBombPer;
		}
	}
}

void GameManager::OnSwapFailed()
{
	m_scoreText->OnSwapFailed();
}

#pragma warning(pop)
