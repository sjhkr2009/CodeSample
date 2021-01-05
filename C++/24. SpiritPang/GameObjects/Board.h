#include "PCH.h"
#include "SpiritFactory.h"
#include <random>
#include <vector>
#pragma once

namespace SpiritPang
{
	class Board : public DX::Actor
	{
	public:
		static const int VerticalBlock{ 7 };
		static const int HorizontalBlock{ 7 };
		static const int TotalBlock{ VerticalBlock * HorizontalBlock };

		const float c_spiritSize{ 80.f };

		enum BlockDirection
		{
			Up = 0,
			Right = 1,
			Down = 2,
			Left = 3
		};

		enum State
		{
			Idle = 0,
			BlockMoving = 1,
			DestroyList = 1 << 1,
			ShuffleWaiting = 1 << 2,
			SpecialBlockSelected = 1 << 3,
			CannotInput = 1 << 4,
			Exploding = CannotInput | (1 << 5),
			SpawnBomb = 1 << 6
		};

	public:
		Board(float spacingX, float spacingY);
		~Board();

		Spirit* operator [] (int index)
		{
			return m_spirits[index];
		};

	private:
		Spirit* m_spirits[TotalBlock]{};
		int		m_waitingBlock[HorizontalBlock]{};

		std::list<int>		m_movingIndex{};
		std::vector<int>	m_foundIndex{};
		std::list<Spirit*>	m_destroyed{};

		std::map<int, std::list<int>> m_destroyByBomb{};

		Spirit* m_selectedSpirit{};
		float	m_clickedX{};
		float	m_clickedY{};

		int		m_indexDelta[4];

		int		m_state{};
		float	m_eventTimer{};

		std::random_device				m_rdevice;
		std::mt19937					m_random;
		std::uniform_int_distribution<> m_dist;

		std::function<void(int)>		m_onBlockDestroy{};
		std::function<void()>			m_onSwapFailed{};

	public:
		void Update(double delta) override;

		void SetSpiritPos(Spirit* spirit, int index) { spirit->m_index = index, spirit->SetPosition(GetSpiritPosX(index), GetSpiritPosY(index)); }
		void SetEventOnBlockDestroy(std::function<void(int)> evt) { m_onBlockDestroy = evt; }
		void SetEventOnSwapFailed(std::function<void()> evt) { m_onSwapFailed = evt; }

		int GetState() const { return m_state; }
		void BombSpawn() { m_state |= State::SpawnBomb; }

	private:
		void OnSelectIn(Spirit* selected, float x, float y);
		void OnSelectOut(float x, float y);

		void SwapSpirit(BlockDirection dir);

		float GetSpiritPosX(int index) { return GetPosition().x + (c_spiritSize * (index % HorizontalBlock)); }
		float GetSpiritPosY(int index) { return GetPosition().y + (c_spiritSize * (index / HorizontalBlock)); }

		bool IsMoving();

		bool IsAdjacentIndex(int origin, int target);
		bool IsValidIndex(int index) { return (m_spirits[index] != nullptr) && (m_spirits[index]->IsActive()) && (m_spirits[index]->m_index == index); }
		void SwapIndex(int indexA, int indexB, bool movePos = false);

		bool CheckMatch3(int index);
		int CheckMatchCount(int originIndex, bool isHorizontal, int maxCount = 3);
		int FindAllAdjacentSpirits(int originIndex, bool additionalFind = false);

		int DestroyBlock();
		void FillBlock();
		Spirit* GetAboveBlock(int index);
		void MoveBlock(Spirit* target, int toIndex);

		void Protect3Match();
		bool CanPlay();
		bool ChangeTypeToCanPlay(int index);
		void ShuffleBoard(bool moveLerp = true);

		void OnBlockDestory(int count) { m_onBlockDestroy(count); }
		void OnSwapFailed() { m_onSwapFailed(); }

		void OnSpecialBlockSelected(float x, float y);
		void OnBombClicked(int index);
		void BombExplosion();
		
	};
}
