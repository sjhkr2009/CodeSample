#pragma once

#include "Card.h"
#include <list>

namespace Solitaire
{
	class GameLogic
	{
		const int BOARD_ROW{ 5 };
		const int BOARD_COLUMN{ 8 };
		const Gdiplus::RectF COUNT_RECT{ 885.f, 60.f, 120.f, 30.f };

		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;

		// 생성자가 없을 때는 선언에서 멤버 변수를 초기화해준다.
		int		mFlipCount{};
		HWND	mHwnd{};
		Card*	mpSelectedCard{};

	public:
		void Init(HWND hwnd);
		void Release();

		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);

	private:
		void CreateCards(HWND hwnd);
	};
}

