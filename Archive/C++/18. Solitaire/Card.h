#pragma once

#include <memory>
#include <Windows.h>
#include <gdiplus.h>
// pragma comment�� �������� �ƴ� ��ũ �������� �ʿ��ϹǷ�, cpp ���Ͽ��� ����Ѵ�.

// ��������� using namespace�� ������� �� ��.

namespace Solitaire
{
	enum class Type
	{
		Wolf,
		Dragon,
		Bear
	};
	
	class Card
	{
	private:
		//Gdiplus::Image	mBack;	// �̹����� �⺻ �����ڸ� ����� �� ������ ����Ǿ� �ִ�.
		//Gdiplus::Image	mFront;	// �̷� Ŭ������ �Ű������� �ְų�, �����ͷ� �����ؾ� �Ѵ�.
		
		// ī���� ����/�ĸ� �̹���
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;

		int		mX;			// ī���� ��ġ
		int		mY;
		bool	mIsFront{ false };	// ī�� ��/�޸� ����
		Type	mType;		// ī���� ����
		HWND	mHwnd;		// ���� �������� ���α׷� â
		int		mIndex;		// ī���� ID ������ �� �ε���

	public:
		Card(HWND hwnd, int index, Type type, int x, int y);

		// ī�� �̹����� �׸��� �Լ�
		void Draw(Gdiplus::Graphics& graphics);
		// Ŭ���� �߻����� �� ȣ��Ǿ�, �� ī�尡 Ŭ���Ǿ��ٸ� ������ �� true�� ��ȯ�Ѵ�.
		bool CheckClicked(int mouseX, int mouseY);
		// ī�带 ������ ó���� �Ѵ�.
		void Flip(bool isFront);
		// ī�带 ������ �� ȣ���Ѵ�. ����� ī�� �̹����� �ٽ� ȭ�鿡 �׸���.
		void Invalidate();

		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}