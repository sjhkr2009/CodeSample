#include "Card.h"
#include <string>

namespace Solitaire
{
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd }, mIndex{ index }, mType{ type }, mX{ x }, mY{ y }
	{
		std::wstring filename;
		switch (mType)	// 멤버 변수가 함수 내에서 변경될 경우를 대비해서, 대입 후에는 매개변수 대신 멤버 변수를 쓰는 것이 낫다.
		{
			case Solitaire::Type::Wolf:
				filename = L"Data/card_creature_wolf.png";
				break;
			case Solitaire::Type::Dragon:
				filename = L"Data/card_creature_dragon.png";
				break;
			case Solitaire::Type::Bear:
				filename = L"Data/card_creature_bear.png";
				break;
			default:
				break;
		}
		mBack = std::make_unique<Gdiplus::Image>(L"Data/card_back.png");
		mFront = std::make_unique<Gdiplus::Image>(filename.c_str()); // char*로 변환할 때는  string.c_str()을 사용한다.
	}
	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			// 앞면 그리기
			// 유니크 포인터를 일반 포인터로 변환할 때는 .get()을 사용 (소유권은 넘어가지 않음)
			// 화살표 연산자는 유니크 포인터에도 오버로딩되어 있으니, .get()->func()로 사용할 필요가 없다.
			graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
		}
		else
		{
			// 뒷면 그리기
			graphics.DrawImage(mBack.get(), mX, mY, mBack->GetWidth(), mBack->GetHeight());
		}
	}
	bool Card::CheckClicked(int mouseX, int mouseY)
	{
		Gdiplus::RectF rct(
			static_cast<Gdiplus::REAL>(mX),
			static_cast<Gdiplus::REAL>(mY),
			static_cast<Gdiplus::REAL>(mBack->GetWidth()), 
			static_cast<Gdiplus::REAL>(mBack->GetHeight())
		);
		
		// 카드 영역 안에 마우스 좌표가 있는지 if문으로 확인한다.
		// 직접 영역을 구해서 입력할수도 있으나, GDI+의 RectF에 있는 Contains 함수를 활용하면 영역 내에 있는지 확인할 수 있다.
		if(rct.Contains(static_cast<Gdiplus::REAL>(mouseX), static_cast<Gdiplus::REAL>(mouseY)))
		/*if ((mX <= mouseX && static_cast<int>(mX + mBack->GetWidth()) >= mouseX) &&
			mY <= mouseY && static_cast<int>(mY + mBack->GetHeight()) >= mouseY)*/
		{
			Flip(!mIsFront);
			return true;
		}

		return false;
	}
	void Card::Flip(bool isFront)
	{
		mIsFront = isFront;

		Invalidate();
	}
	void Card::Invalidate()
	{
		RECT rct{
			mX, mY,
			mX + mBack->GetWidth(),
			mY + mBack->GetHeight()
		};
		
		// InvalidateRect: 특정 영역을 무효화해서 다시 그린다. 윈도우 프로그래밍에만 있는 개념. (실제 게임은 초당 전체화면을 60회씩 그려야 하므로)
		// 이렇게 일부만 삭제한 공간을 Dirty Region 이라고 한다.
		// 매개 변수: HWND, 삭제할 공간, 해당 공간 삭제 여부
		InvalidateRect(mHwnd, &rct, false);
	}
}
