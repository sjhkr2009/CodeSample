#include "Card.h"
#include <string>

namespace Solitaire
{
	Card::Card(HWND hwnd, int index, Type type, int x, int y) :
		mHwnd{ hwnd }, mIndex{ index }, mType{ type }, mX{ x }, mY{ y }
	{
		std::wstring filename;
		switch (mType)	// ��� ������ �Լ� ������ ����� ��츦 ����ؼ�, ���� �Ŀ��� �Ű����� ��� ��� ������ ���� ���� ����.
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
		mFront = std::make_unique<Gdiplus::Image>(filename.c_str()); // char*�� ��ȯ�� ����  string.c_str()�� ����Ѵ�.
	}
	void Card::Draw(Gdiplus::Graphics& graphics)
	{
		if (mIsFront)
		{
			// �ո� �׸���
			// ����ũ �����͸� �Ϲ� �����ͷ� ��ȯ�� ���� .get()�� ��� (�������� �Ѿ�� ����)
			// ȭ��ǥ �����ڴ� ����ũ �����Ϳ��� �����ε��Ǿ� ������, .get()->func()�� ����� �ʿ䰡 ����.
			graphics.DrawImage(mFront.get(), mX, mY, mFront->GetWidth(), mFront->GetHeight());
		}
		else
		{
			// �޸� �׸���
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
		
		// ī�� ���� �ȿ� ���콺 ��ǥ�� �ִ��� if������ Ȯ���Ѵ�.
		// ���� ������ ���ؼ� �Է��Ҽ��� ������, GDI+�� RectF�� �ִ� Contains �Լ��� Ȱ���ϸ� ���� ���� �ִ��� Ȯ���� �� �ִ�.
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
		
		// InvalidateRect: Ư�� ������ ��ȿȭ�ؼ� �ٽ� �׸���. ������ ���α׷��ֿ��� �ִ� ����. (���� ������ �ʴ� ��üȭ���� 60ȸ�� �׷��� �ϹǷ�)
		// �̷��� �Ϻθ� ������ ������ Dirty Region �̶�� �Ѵ�.
		// �Ű� ����: HWND, ������ ����, �ش� ���� ���� ����
		InvalidateRect(mHwnd, &rct, false);
	}
}
