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
/// ���콺 Ŭ�� �� ���� ���� �������� ���� �� ������ ���¿� ���� ������ �����մϴ�.
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

// �� ������ ���콺 Ŭ�� �� ȣ��˴ϴ�. �ش� ���� Ŭ�� ������ ��ǥ�� �����մϴ�.
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
/// ���콺�� ���� ��� ȣ��˴ϴ�. �������� ������ ������ ��� ������ Ȯ���� �� SwapSpirit�� ȣ���մϴ�.
/// (���õ� ���� ���ų� ������ �� ���� ��� �ƹ� ���۵� ���� �ʽ��ϴ�)
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

	// ���콺�� ������ �Ÿ��� 0.7ĭ ũ�⺸�� ������, �ƹ� ���۵� ���� �ʴ´�.
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
/// ���� ���õ� ��(m_selectedSpirit)�� �Է��� ������ ������ ���� �ڸ��� �ٲߴϴ�.
/// �ڸ��� �ٲ� �� 3ĭ�� ��ġ�� ���� ������ ������ ��� ���� �ı��ϰ�, ������ ���� �ڸ��� ���ư��ϴ�.
/// (�Է��� ���⿡ ���� ���ٸ� �ƹ� ���۵� ���� �ʽ��ϴ�)
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
/// �Է��� �� �ε����� �ش��ϴ� ���� ã��, �ش� ��(Spirit)�� �ε����� ���� �ٲߴϴ�.
/// movePos�� true�� ��� ��ġ�� �ٲ�ϴ�.
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

// m_movingIndex ����Ʈ�� ������� �����̴� ���� �ִ��� Ȯ���մϴ�.
// �̵����� ���� �ϳ��� �ִٸ� true��, ���ٸ� ����Ʈ�� ���� false�� ��ȯ�մϴ�.
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
/// �� �ε����� ������ ������ üũ�մϴ�.
/// (��� �ε����� �迭 ������ �ʰ����� ��� �׻� false�� ��ȯ�մϴ�)
/// </summary>
/// <returns>target�� ��ȿ�� �ε����̸� origin�� ���� �Ǵ� ���η� ������ ���� ��� true�� ��ȯ�մϴ�</returns>
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

// �Է��� �ε��� ��ǥ�� �ִ� �����κ��� 3ĭ �̻� ������ ��ġ�� ������ ���� �ִ��� Ȯ���մϴ�.
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
/// �ش� ��ǥ���� ���� �Ǵ� ���η� ������ ��ġ�� ����� ������ ��ȯ�մϴ�.
/// </summary>
/// <param name="originIndex">: Ž���� ������ ������ �ε��� ��ǥ</param>
/// <param name="isHorizontal">: true�� ���� ����, false�� ���� �������� Ž���մϴ�.</param>
/// <param name="maxCount">: ����ȭ�� ���� �ش� ������ŭ�� ��Ҹ� ã���� ��� ��� ��ȯ�մϴ�.</param>
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
/// �Է��� �ε��� ��ǥ�κ��� ������ ��� ������ Ÿ���� Spirit�� ã�Ƽ� m_foundIndex�� �����մϴ�.
/// </summary>
/// <param name="originIndex">: Ž���� ������ �ε��� ��ǥ</param>
/// <param name="additionalFind">
/// : true�� ������ m_foundList�� ���� ã�� ��Ҹ� �߰��մϴ�. (�ߺ� �߰����� �ʽ��ϴ�)
///	false�� ����Ʈ�� �ʱ�ȭ�� �� ���� ã�� ��Ҹ� �����մϴ�.</param>
/// <returns>Ž�� �� ����Ʈ�� ���� ������ ��ȯ�մϴ�</returns>
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

// m_foundIndex�� ��� ����� �ı��մϴ�. �� ���� m_destroyed�� �����˴ϴ�.
// �ı��� ������ ��� ������ ��ȯ�˴ϴ�.
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

// �� ������ ���� ä�� �ְ�, ä�� ���� ���� �ٽ� 3���� ��ġ�� ��� �ı� ������ ����մϴ�.
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

// �Է��� �ε����� ���� ���� ���ʿ� �ִ� ���� �� ���� ����� ���� ��ȯ�մϴ�.
// ������ ��� �ִٸ� �ı��� �� ����Ʈ���� �� ���� ��Ҹ� ���� ��ȯ�մϴ�.
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

// ���� ������ �ε��� ��ǥ�� ��ġ�� �̵���ŵ�ϴ�.
// �̵��� �ε����� �ٸ� ���� �ִٸ� �����������, ���� �ε������� �ش� ���� ���������� �ʽ��ϴ�.
void Board::MoveBlock(Spirit* target, int toIndex)
{
	m_spirits[toIndex] = target;
	target->m_index = toIndex;

	target->SetDestination(GetSpiritPosX(toIndex), GetSpiritPosY(toIndex));
	
	m_movingIndex.push_back(toIndex);
	m_state |= State::BlockMoving;
}

// �ʿ� 3���� ���� ������ �ִ� ��Ȳ�� �����մϴ�. ���� ���� �� �Ǵ� ���带 ������ �� ȣ��˴ϴ�.
// 3���� ��ġ�Ǿ� �ִ� ���� ������ ���� �������� ���� ������ Ÿ������ ����˴ϴ�.
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

// ������ ������ �� �ִ��� üũ�մϴ�.
// �� �̻� � ������ε� �Ͷ߸� �� �ִ� ����� ���ٸ� false�� ��ȯ�մϴ�.
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
					Utility::DebugString(L"[Board::CanPlay] �÷��̰� �����մϴ�. (%d�� %d�� ��ġ ������ �����մϴ�)", now, next);
					return true;
				}
			}
		}
	}
	Utility::DebugString(L"[Board::CanPlay] �÷��̸� ����� �� �����ϴ�!");
	return result;
}

// ������ ���� Ÿ���� �÷��̰� ������ �Ϲ� Ÿ�� ���� �� ������ �ϳ��� �����մϴ�.
// � Ÿ������ �����ص� ���� ������ �Ұ��ϴٸ� false�� ��ȯ�մϴ�.
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
		Utility::DebugString(L"[Board::ChangeTypeToCanPlay] %d�� ���� Ÿ���� %d ���� %d ���� ����Ǿ����ϴ�.",
			index, static_cast<int>(originType), validType[0]);
		return true;
	}
}

// ��� ���� �����ϴ�. init�� true�� ��� 3-Match�� �Ͼ�� �ʵ��� �����ϰ�, false�� ��� ���� �� 3-Match�� ���� �Ͷ߸��ϴ�.
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

// Ư�� ���� ���õ� ����� ó���� ����մϴ�.
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

// ��ź ���� Ŭ�� �� ȣ��˴ϴ�. ���� ���� ��ź ���� �Ÿ��� ���ؼ� m_destroyByBomb�� �ְ�, State::Exploding ���·� �����մϴ�.
// ���� ���: ��ź ���� ������ ���� �� + �� �Ʒ� ���� ��
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

// ���� ��(State::Exploding�� ��)�� ȣ��˴ϴ�. ���� �������� ����� �ð��� üũ�մϴ�.
// ��� �ð��� ���� ��ź ���� �Ÿ��� ����� ������ ���������� ���߽�ŵ�ϴ�. �Ϸ�Ǹ� State::Exploding ���¸� �����մϴ�.
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
