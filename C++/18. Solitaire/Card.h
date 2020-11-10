#pragma once

#include <memory>
#include <Windows.h>
#include <gdiplus.h>
// pragma comment는 컴파일이 아닌 링크 과정에서 필요하므로, cpp 파일에서 사용한다.

// 헤더에서는 using namespace를 사용하지 말 것.

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
		//Gdiplus::Image	mBack;	// 이미지는 기본 생성자를 사용할 수 없도록 설계되어 있다.
		//Gdiplus::Image	mFront;	// 이런 클래스는 매개변수를 넣거나, 포인터로 선언해야 한다.
		
		// 카드의 전면/후면 이미지
		std::unique_ptr<Gdiplus::Image> mBack;
		std::unique_ptr<Gdiplus::Image> mFront;

		int		mX;			// 카드의 위치
		int		mY;
		bool	mIsFront{ false };	// 카드 앞/뒷면 여부
		Type	mType;		// 카드의 종류
		HWND	mHwnd;		// 현재 실행중인 프로그램 창
		int		mIndex;		// 카드의 ID 역할을 할 인덱스

	public:
		Card(HWND hwnd, int index, Type type, int x, int y);

		// 카드 이미지를 그리는 함수
		void Draw(Gdiplus::Graphics& graphics);
		// 클릭이 발생했을 때 호출되어, 이 카드가 클릭되었다면 뒤집은 후 true를 반환한다.
		bool CheckClicked(int mouseX, int mouseY);
		// 카드를 뒤집는 처리를 한다.
		void Flip(bool isFront);
		// 카드를 뒤집은 후 호출한다. 변경된 카드 이미지를 다시 화면에 그린다.
		void Invalidate();

		Type GetType() const { return mType; }
		int GetIndex() const { return mIndex; }
	};
}