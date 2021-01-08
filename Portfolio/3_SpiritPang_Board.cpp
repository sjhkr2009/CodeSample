#include "PCH.h"
#include "Board.h"
#include <queue>

using namespace DirectX;
using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)

Board::Board(float spacingX, float spacingY) :
	Actor(L"Assets/boardBackground.png"),
	m_rdevice{},
	m_random(m_rdevice()),
	m_dist(0, static_cast<int>(Spirit::Type::Count) - 1),
	m_indexDelta{ -HorizontalBlock, 1, HorizontalBlock, -1 }
{
	SetPivot(0.f, 0.f);
	SetPosition(spacingX, spacingY);
	float targetSize = c_spiritSize * VerticalBlock;
	SetScale((targetSize) / GetOriginSize().x);

	Pause();

	for (int i = 0; i < TotalBlock; i++)
	{
		int type = m_dist(m_random);
		m_spirits[i] = SpiritFactory::CreateSpirit(type, c_spiritSize);
		SetSpiritPos(m_spirits[i], i);
	}
	Protect3Match();

	while (CanPlay() == false)
	{
		ShuffleBoard(false);
	}
}

void Board::Reset()
{
	m_state = 0;
	Pause();
	for (int i = 0; i < TotalBlock; i++)
	{
		int type = m_dist(m_random);
		m_spirits[i]->SetType(static_cast<Spirit::Type>(type));
		SetSpiritPos(m_spirits[i], i);
	}
	Protect3Match();

	while (CanPlay() == false)
	{
		ShuffleBoard(false);
	}
}

Board::~Board()
{
}

/// <summary>
/// 마우스 클릭 및 해제 시의 스와이프 동작 및 보드의 상태에 따른 동작을 실행합니다.
/// </summary>
void Board::Update(double delta)
{
	Actor::Update(delta);

#pragma region Debug
	if (InputManager::Instance().IsKeyDown(Keyboard::D))
	{
		for (int i = 0; i < TotalBlock; i++)
		{
			m_spirits[i]->Debug();
		}
	}
	if (InputManager::Instance().IsKeyDown(Keyboard::F))
	{
		CanPlay();
	}
	if (InputManager::Instance().IsKeyDown(Keyboard::R))
	{
		Reset();
	}
	if (InputManager::Instance().IsKeyDown(Keyboard::P))
	{
		m_state = (m_state & State::Paused) ?
			m_state & ~(State::Paused) : m_state | State::Paused;
	}
#pragma endregion
	
#pragma region State Control
	if (m_state & State::Paused || m_state & State::Gameover)
	{
		return;
	}
	
	if ((m_state & State::BlockMoving) && !IsMoving())
	{
		m_state &= ~(State::BlockMoving);
		if (m_state & State::DestroyList)
		{
			DestroyBlock();
			FillBlock();
		}
		if (m_state & State::ShuffleWaiting)
		{
			ShuffleBoard();
		}
	}

	if (m_state & State::CannotInput)
	{
		m_eventTimer += delta;
		if (m_state & State::Exploding)
		{
			BombExplosion();
		}
		return;
	}
#pragma endregion

#pragma region Input

	if (InputManager::Instance().IsPointerDown())
	{
		float x = InputManager::Instance().GetMousePos().x;
		float y = InputManager::Instance().GetMousePos().y;
		Utility::DebugString(L"[Board::Update] Mouse Clicked : %0.1f, %0.1f", x, y);

		for (int i = 0; i < TotalBlock; i++)
		{
			if (m_spirits[i]->Contain(x, y))
			{
				OnSelectIn(m_spirits[i], x, y);
				return;
			}
		}
	}
	else if (InputManager::Instance().IsPointerUp())
	{
		float x = InputManager::Instance().GetMousePos().x;
		float y = InputManager::Instance().GetMousePos().y;
		Utility::DebugString(L"[Board::Update] Mouse Released : %0.1f, %0.1f", x, y);

		OnSelectOut(x, y);
		m_selectedSpirit = nullptr;
	}
#pragma endregion
}

// 블럭 위에서 마우스 클릭 시 호출됩니다. 해당 블럭과 클릭 지점의 좌표를 저장합니다.
void Board::OnSelectIn(Spirit* selected, float x, float y)
{
	m_selectedSpirit = selected;
	m_clickedX = x;
	m_clickedY = y;

	if (!m_selectedSpirit->IsNormalType())
	{
		m_state |= State::SpecialBlockSelected;
	}
}

/// <summary>
/// 마우스를 뗐을 경우 호출됩니다. 스와이프 동작이 감지된 경우 방향을 확인한 후 SwapSpirit을 호출합니다.
/// (선택된 블럭이 없거나 움직일 수 없을 경우 아무 동작도 하지 않습니다)
/// </summary>
void Board::OnSelectOut(float x, float y)
{
	if (m_selectedSpirit == nullptr || !m_selectedSpirit->IsActive() || m_selectedSpirit->IsMoving())
	{
		return;
	}
	if (m_state & State::SpecialBlockSelected)
	{
		OnSpecialBlockSelected(x, y);
		m_state &= ~(State::SpecialBlockSelected);
		return;
	}
	XMFLOAT2 mouseDir{ m_clickedX - x, m_clickedY - y };

	float distX = std::abs(mouseDir.x);
	float distY = std::abs(mouseDir.y);

	// 마우스를 움직인 거리가 0.7칸 크기보다 작으면, 아무 동작도 하지 않는다.
	if (std::pow(distX, 2) + std::pow(distY, 2) < std::pow(c_spiritSize * 0.7f, 2))
	{
		return;
	}
	Utility::DebugString(L"[Board::OnSelectOut] Swiping Complete : %0.1f, %0.1f", mouseDir.x, mouseDir.y);

	BlockDirection swapDir;
	if (distX > distY)
	{
		swapDir = (mouseDir.x < 0) ? BlockDirection::Right : BlockDirection::Left;
	}
	else
	{
		swapDir = (mouseDir.y < 0) ? BlockDirection::Down : BlockDirection::Up;
	}

	SwapSpirit(swapDir);
}

/// <summary>
/// 현재 선택된 블럭(m_selectedSpirit)과 입력한 방향의 인접한 블럭의 자리를 바꿉니다.
/// 자리를 바꾼 후 3칸이 매치된 블럭이 있으면 인접한 모든 블럭을 파괴하고, 없으면 원래 자리로 돌아갑니다.
/// (입력한 방향에 블럭이 없다면 아무 동작도 하지 않습니다)
/// </summary>
void Board::SwapSpirit(BlockDirection dir)
{
	int indexDelta = m_indexDelta[static_cast<int>(dir)];
	Utility::DebugString(L"[Board::SwapSpirit] Block Swap : %d (0=Up, 1=Right, 2=Down, 3=Left)", dir);

	int originIndex = m_selectedSpirit->m_index;
	int targetIndex = originIndex + indexDelta;
	if (!IsAdjacentIndex(originIndex, targetIndex) || m_spirits[targetIndex] == nullptr ||
		!m_spirits[targetIndex]->IsActive() || m_spirits[targetIndex]->IsMoving())
	{
		return;
	}

	SwapIndex(originIndex, targetIndex, true);

	bool matchOrigin = CheckMatch3(originIndex);
	bool matchOther = CheckMatch3(targetIndex);

	if (!(matchOrigin || matchOther))
	{
		m_spirits[originIndex]->SetState(Spirit::State::ReturnOnMoveEnd);
		m_spirits[targetIndex]->SetState(Spirit::State::ReturnOnMoveEnd);
		SwapIndex(originIndex, targetIndex);
		OnSwapFailed();
		return;
	}

	if (matchOrigin)
	{
		FindAllAdjacentSpirits(originIndex);
	}
	if (matchOther)
	{
		FindAllAdjacentSpirits(targetIndex, true);
	}
	m_state |= State::DestroyList;
}

/// <summary>
/// 입력한 두 인덱스에 해당하는 블럭을 찾아, 해당 블럭(Spirit)의 인덱스를 서로 바꿉니다.
/// movePos가 true일 경우 위치도 바뀝니다.
/// </summary>
void Board::SwapIndex(int indexA, int indexB, bool movePos)
{
	Spirit* temp = m_spirits[indexA];
	m_spirits[indexA] = m_spirits[indexB];
	m_spirits[indexB] = temp;

	m_spirits[indexA]->m_index = indexA;
	m_spirits[indexB]->m_index = indexB;

	if (movePos)
	{
		m_spirits[indexA]->SetDestination(GetSpiritPosX(indexA), GetSpiritPosY(indexA));
		m_spirits[indexB]->SetDestination(GetSpiritPosX(indexB), GetSpiritPosY(indexB));

		m_movingIndex.push_back(indexA);
		m_movingIndex.push_back(indexB);

		m_state |= State::BlockMoving;
	}
}

// m_movingIndex 리스트를 기반으로 움직이는 블럭이 있는지 확인합니다.
// 이동중인 블럭이 하나라도 있다면 true를, 없다면 리스트를 비우고 false를 반환합니다.
bool Board::IsMoving()
{
	for (auto& idx : m_movingIndex)
	{
		if (m_spirits[idx]->IsMoving())
		{
			return true;
		}
	}

	m_movingIndex.clear();
	return false;
}

/// <summary>
/// 두 인덱스가 인접한 블럭인지 체크합니다.
/// (대상 인덱스가 배열 범위를 초과했을 경우 항상 false를 반환합니다)
/// </summary>
/// <returns>target이 유효한 인덱스이며 origin과 가로 또는 세로로 인접한 블럭일 경우 true를 반환합니다</returns>
bool Board::IsAdjacentIndex(int origin, int target)
{
	if (target < 0 || target >= TotalBlock)
	{
		return false;
	}

	int difference{ std::abs(origin - target) };
	if (difference != 1 && difference != HorizontalBlock)
	{
		return false;
	}

	if (difference == 1 &&
		(target / HorizontalBlock) != (origin / HorizontalBlock))
	{
		return false;
	}
	
	return true;
}

// 입력한 인덱스 좌표에 있는 블럭으로부터 3칸 이상 나란히 배치된 동일한 블럭이 있는지 확인합니다.
bool Board::CheckMatch3(int originIndex)
{
	if (CheckMatchCount(originIndex, true) >= 3)
	{
		return true;
	}
	if (CheckMatchCount(originIndex, false) >= 3)
	{
		return true;
	}

	return false;
}

/// <summary>
/// 해당 좌표에서 가로 또는 세로로 나란히 배치된 요소의 개수를 반환합니다.
/// </summary>
/// <param name="originIndex">: 탐지를 시작할 지점의 인덱스 좌표</param>
/// <param name="isHorizontal">: true면 가로 방향, false면 세로 방향으로 탐지합니다.</param>
/// <param name="maxCount">: 최적화를 위해 해당 개수만큼의 요소를 찾았을 경우 즉시 반환합니다.</param>
int Board::CheckMatchCount(int originIndex, bool isHorizontal, int maxCount)
{
	int matchCount{ 1 };
	Spirit::Type originType{ m_spirits[originIndex]->GetType() };

	int i = isHorizontal ? 1 : 0;
	for (; i < 4; i += 2)
	{
		int delta{ m_indexDelta[i] };
		int currentIndex{ originIndex };
		int targetIndex{ originIndex + delta };

		while (matchCount < maxCount && IsAdjacentIndex(currentIndex, targetIndex))
		{
			if (m_spirits[targetIndex]->IsActive() &&
				m_spirits[targetIndex]->GetType() == originType)
			{
				matchCount++;
				currentIndex = targetIndex;
				targetIndex += delta;
			}
			else
			{
				break;
			}
		}
		
		if (matchCount >= maxCount)
		{
			break;
		}
	}
	return matchCount;
}

/// <summary>
/// 입력한 인덱스 좌표로부터 인접한 모든 동일한 타입의 Spirit을 찾아서 m_foundIndex에 저장합니다.
/// </summary>
/// <param name="originIndex">: 탐색을 시작할 인덱스 좌표</param>
/// <param name="additionalFind">
/// : true면 현재의 m_foundList에 새로 찾은 요소를 추가합니다. (중복 추가되지 않습니다)
///	false면 리스트를 초기화한 후 새로 찾은 요소만 저장합니다.</param>
/// <returns>탐색 후 리스트의 원소 개수를 반환합니다</returns>
int Board::FindAllAdjacentSpirits(int originIndex, bool additionalFind)
{
	Spirit::Type originType{ m_spirits[originIndex]->GetType() };

	bool checked[TotalBlock]{};
	std::queue<int> reserved{};
	if (additionalFind)
	{
		for (auto& e : m_foundIndex)
		{
			checked[e] = true;
		}
	}
	else
	{
		m_foundIndex.clear();
	}
	if (checked[originIndex])
	{
		return 0;
	}

	checked[originIndex] = true;
	reserved.push(originIndex);

	while (!reserved.empty())
	{
		int now = reserved.front();
		reserved.pop();

		m_foundIndex.push_back(now);
		m_spirits[now]->SetParticleType(ParticlePang::ParticleType::Disappear);
		Utility::DebugString(L"[Board::FindAllAdjacentSpirits] Find Same Adjacent Spirit : %d", now);

		for (int i = 0; i < 4; i++)
		{
			int next = now + m_indexDelta[i];

			if (!IsAdjacentIndex(now, next) || checked[next])
			{
				continue;
			}
			if (m_spirits[next]->GetType() == originType)
			{
				checked[next] = true;
				reserved.push(next);
			}
		}
	}
	return static_cast<int>(m_foundIndex.size());
}

// m_foundIndex의 모든 블록을 파괴합니다. 각 블럭은 m_destroyed에 보관됩니다.
// 파괴에 성공한 블록 개수가 반환됩니다.
int Board::DestroyBlock()
{
	m_state &= ~(State::DestroyList);
	std::map<Spirit::Type, int> destroyedCount{};
	int count{};

	for (auto& idx : m_foundIndex)
	{
		if (m_spirits[idx]->Destroy())
		{
			auto result = destroyedCount.insert(std::make_pair(m_spirits[idx]->GetType(), 1));
			if (!result.second)
			{
				result.first->second++;
			}
			m_destroyed.push_back(m_spirits[idx]);
		}
	}

	for (auto& type : destroyedCount)
	{
		OnBlockDestory(type.second);
		count += type.second;
	}

	return count;
}

// 빈 공간에 블럭을 채워 넣고, 채워 넣은 블럭이 다시 3개가 매치될 경우 파괴 로직을 명령합니다.
void Board::FillBlock()
{
	for (int x = 0; x < HorizontalBlock; ++x)
	{
		for (int y = VerticalBlock - 1; y >= 0; --y)
		{
			int idx{ (y * HorizontalBlock) + x };
			if (IsValidIndex(idx))
			{
				continue;
			}
			
			Spirit* pSpirit{ GetAboveBlock(idx) };
			pSpirit->SetActive(true);

			MoveBlock(pSpirit, idx);
		}
	}
	for (int i = 0; i < HorizontalBlock; i++)
	{
		m_waitingBlock[i] = 0;
	}

	bool canPlay = CanPlay();
	if (!canPlay)
	{
		for (auto& idx : m_movingIndex)
		{
			if (ChangeTypeToCanPlay(idx))
			{
				canPlay = true;
				break;
			}
		}
		if (!canPlay)
		{
			m_state |= State::ShuffleWaiting;
			m_foundIndex.clear();
			return;
		}
	}
	
	m_foundIndex.clear();
	for (auto& idx : m_movingIndex)
	{
		bool match{ CheckMatch3(idx) };
		if (match)
		{
			FindAllAdjacentSpirits(idx, true);
			m_state |= State::DestroyList;
		}
	}
}

// 입력한 인덱스와 같은 열의 위쪽에 있는 블럭들 중 가장 가까운 블럭을 반환합니다.
// 위쪽이 비어 있다면 파괴된 블럭 리스트에서 맨 앞의 요소를 꺼내 반환합니다.
Spirit* Board::GetAboveBlock(int index)
{
	Spirit* pSpirit{ nullptr };

	for (int idx = (index - HorizontalBlock); idx >= 0; idx -= HorizontalBlock)
	{
		if (IsValidIndex(idx))
		{
			pSpirit = m_spirits[idx];
			m_spirits[idx] = nullptr;
			break;
		}
	}

	if (pSpirit == nullptr)
	{
		pSpirit = m_destroyed.front();
		m_destroyed.pop_front();

		int y{ index % VerticalBlock };
		m_waitingBlock[y]++;
		pSpirit->SetPosition(
			GetSpiritPosX(index),
			GetPosition().y - (c_spiritSize * m_waitingBlock[y])
		);

		if (m_state & State::SpawnBomb)
		{
			pSpirit->SetType(Spirit::Type::Special_Bomb);
			m_state &= ~(State::SpawnBomb);
		}
		else 
		{
			pSpirit->SetType(static_cast<Spirit::Type>(m_dist(m_random)));
		}
	}

	return pSpirit;
}

// 블럭을 지정한 인덱스 좌표의 위치로 이동시킵니다.
// 이동할 인덱스에 다른 블럭이 있다면 덮어씌워지지만, 기존 인덱스에서 해당 블럭이 삭제되지는 않습니다.
void Board::MoveBlock(Spirit* target, int toIndex)
{
	m_spirits[toIndex] = target;
	target->m_index = toIndex;

	target->SetDestination(GetSpiritPosX(toIndex), GetSpiritPosY(toIndex));
	
	m_movingIndex.push_back(toIndex);
	m_state |= State::BlockMoving;
}

// 맵에 3개의 블럭이 나란히 있는 상황을 방지합니다. 게임 시작 시 또는 보드를 리셋할 때 호출됩니다.
// 3개가 매치되어 있는 블럭은 인접한 블럭과 동일하지 않은 임의의 타입으로 변경됩니다.
void Board::Protect3Match()
{
	std::vector<int> existType{};

	for (int now = 0; now < TotalBlock; now++)
	{
		if (!CheckMatch3(now))
		{
			continue;
		}

		existType.clear();
		for (int i = 0; i < 4; i++)
		{
			int next = now + m_indexDelta[i];
			if (!IsAdjacentIndex(now, next))
			{
				continue;
			}
			existType.push_back(static_cast<int>(m_spirits[next]->GetType()));
		}

		int newType{ m_dist(m_random) };
		while (true)
		{
			bool isExist{ false };
			for (int i = 0; i < existType.size(); i++)
			{
				if (newType == existType[i])
				{
					isExist = true;
					break;
				}
			}
			if (!isExist)
			{
				break;
			}
			else
			{
				newType = m_dist(m_random);
			}
		}
		m_spirits[now]->SetType(static_cast<Spirit::Type>(newType));
	}
}

// 게임을 지속할 수 있는지 체크합니다.
// 더 이상 어떤 방식으로도 터뜨릴 수 있는 블록이 없다면 false를 반환합니다.
bool Board::CanPlay()
{
	int delta[2]{ 
		m_indexDelta[static_cast<int>(BlockDirection::Right)], 
		m_indexDelta[static_cast<int>(BlockDirection::Down)]
	};
	bool result{ false };

	for (int now = 0; now < TotalBlock; now++)
	{
		if ((now % HorizontalBlock) == (HorizontalBlock - 1) ||
			(now / HorizontalBlock) == (VerticalBlock - 1))
		{
			continue;
		}
		if (!m_spirits[now]->IsNormalType())
		{
			return true;
		}

		for (int i = 0; i < 2; i++)
		{
			int next = now + delta[i];
			if (IsAdjacentIndex(now, next))
			{
				SwapIndex(now, next);
				result = (CheckMatch3(now) || CheckMatch3(next));
				SwapIndex(now, next);

				if (result)
				{
					Utility::DebugString(L"[Board::CanPlay] 플레이가 가능합니다. (%d와 %d의 위치 변경이 가능합니다)", now, next);
					return true;
				}
			}
		}
	}
	Utility::DebugString(L"[Board::CanPlay] 플레이를 계속할 수 없습니다!");
	return result;
}

// 지정한 블럭의 타입을 플레이가 가능한 일반 타입 블럭들 중 임의의 하나로 변경합니다.
// 어떤 타입으로 변경해도 게임 진행이 불가하다면 false를 반환합니다.
bool Board::ChangeTypeToCanPlay(int index)
{
	std::vector<int> validType{};
	Spirit::Type originType{ m_spirits[index]->GetType() };

	for (int currentType = 0; currentType < static_cast<int>(Spirit::Type::Count); currentType++)
	{
		if (currentType == static_cast<int>(originType))
		{
			continue;
		}

		m_spirits[index]->SetType(static_cast<Spirit::Type>(currentType), false);

		if (CanPlay())
		{
			validType.push_back(currentType);
		}
	}

	if (validType.empty())
	{
		m_spirits[index]->SetType(originType, false);
		return false;
	}
	else
	{
		std::shuffle(validType.begin(), validType.end(), m_random);
		m_spirits[index]->SetType(static_cast<Spirit::Type>(validType[0]));
		Utility::DebugString(L"[Board::ChangeTypeToCanPlay] %d번 블럭의 타입이 %d 에서 %d 으로 변경되었습니다.",
			index, static_cast<int>(originType), validType[0]);
		return true;
	}
}

// 모든 블럭을 섞습니다. init가 true일 경우 3-Match가 일어나지 않도록 방지하고, false인 경우 섞은 후 3-Match된 블럭을 터뜨립니다.
void Board::ShuffleBoard(bool init)
{
	Spirit* spirits[TotalBlock]{};
	std::vector<int> indexList{};
	for (int i = 0; i < TotalBlock; i++)
	{
		indexList.push_back(i);
		spirits[i] = m_spirits[i];
;	}
	
	std::shuffle(indexList.begin(), indexList.end(), m_random);
	for (int i = 0; i < TotalBlock; i++)
	{
		if (init)
		{
			MoveBlock(spirits[i], indexList[i]);
		}
		else
		{
			m_spirits[indexList[i]] = spirits[i];
			SetSpiritPos(spirits[i], indexList[i]);
		}
	}

	if (!init)
	{
		Protect3Match();
		return;
	}

	m_foundIndex.clear();
	for (int idx = 0; idx < TotalBlock; idx++)
	{
		bool match{ CheckMatch3(idx) };
		if (match)
		{
			FindAllAdjacentSpirits(idx, true);
			m_state |= State::DestroyList;
		}
	}
}

// 특수 블럭이 선택된 경우의 처리를 담당합니다.
void Board::OnSpecialBlockSelected(float x, float y)
{
	switch (m_selectedSpirit->GetType())
	{
		case Spirit::Type::Special_Bomb:
			if (m_selectedSpirit->Contain(x, y))
			{
				OnBombClicked(m_selectedSpirit->m_index);
			}
			break;
		default:
			break;
	}
}

// 폭탄 블럭을 클릭 시 호출됩니다. 폭발 대상과 폭탄 블럭의 거리를 구해서 m_destroyByBomb에 넣고, State::Exploding 상태로 변경합니다.
// 폭발 대상: 폭탄 블럭과 동일한 열의 블럭 + 맨 아래 행의 블럭
void Board::OnBombClicked(int index)
{
	m_spirits[index]->SetParticleType(ParticlePang::ParticleType::BombAttack);

	int tempBoard[TotalBlock]{};
	for (int i = 0; i < TotalBlock; i++)
	{
		if ((i % VerticalBlock) == (index % VerticalBlock) || (i / VerticalBlock) == (VerticalBlock - 1))
		{
			tempBoard[i] = 1;
		}
	}
	tempBoard[index] = 0;
	
	bool checked[TotalBlock]{};
	std::queue<int> reserved{};

	reserved.push(index);
	m_destroyByBomb.clear();
	while (!reserved.empty())
	{
		int now = reserved.front();
		reserved.pop();
		checked[now] = true;

		for (int i = 0; i < 4; i++)
		{
			int next = now + m_indexDelta[i];
			if (!IsAdjacentIndex(now, next) || checked[next] || tempBoard[next] == 0)
			{
				continue;
			}
			tempBoard[next] = tempBoard[now] + 1;
			auto result = m_destroyByBomb.insert(std::make_pair(tempBoard[next], std::list<int>{next}));
			if (!result.second)
			{
				result.first->second.push_back(next);
			}
			reserved.push(next);
		}
	}
	m_state |= State::Exploding;
	OnTimerStop();
	m_eventTimer = 0.f;

	m_foundIndex.clear();
	m_foundIndex.push_back(index);
	DestroyBlock();
}

// 폭발 중(State::Exploding일 때)에 호출됩니다. 폭발 시점에서 경과한 시간을 체크합니다.
// 경과 시간에 따라 폭탄 블럭과 거리가 가까운 블럭부터 순차적으로 폭발시킵니다. 완료되면 State::Exploding 상태를 해제합니다.
void Board::BombExplosion()
{
	if (m_destroyByBomb.empty())
	{
		m_state &= ~(State::Exploding);
		OnTimerStop(false);
		FillBlock();
		return;
	}
	
	std::list<int> destroyed{};
	for (int i = 1; i <= static_cast<int>(m_eventTimer * 10 + 0.5f); i++)
	{
		auto itr = m_destroyByBomb.find(i);
		if (itr == m_destroyByBomb.end())
		{
			continue;
		}
		destroyed.push_back(itr->first);
		for (auto idx : itr->second)
		{
			m_spirits[idx]->SetParticleType(ParticlePang::ParticleType::BombSplash);
			m_foundIndex.push_back(idx);
		}
	}

	if (destroyed.empty())
	{
		return;
	}
	
	for (int key : destroyed)
	{
		m_destroyByBomb.erase(key);
	}
	DestroyBlock();
}

#pragma warning(pop)
