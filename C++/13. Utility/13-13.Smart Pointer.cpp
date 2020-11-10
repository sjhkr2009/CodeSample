#include <iostream>

// 스마트 포인터
// : 메모리 해제를 수동으로 해주지 않아도, 알아서 해제되도록 하는 개념. 포인터를 편하게 쓸 수 있다.

// 3가지 종류가 있다.
// unique_ptr : 고유 포인터. 가리키는 곳을 하나의 포인터만으로 가리킬 때. 고유한 객체들은 이 포인터를 쓴다.
//	 ㄴ 나 자신만 소유권(ownership)을 가지며, 다른 곳에 넘겨줄 수 없다.
// shared_ptr : 공유 포인터. 가리키는 곳을 여러 곳에서 참조할 때
//	 ㄴ 다른 곳에서 함께 소유권을 가질 수 있다.
//	 ㄴ 동일한 몬스터가 여럿 생성되어도 같은 이미지를 참조한다. 이 때 이미지 파일은 shared 포인터로 선언해야 한다.
//	 ㄴ 가리키는 곳은 참조(Reference) 카운트를 가지며, 참조가 0이 되면 메모리를 해제한다.
// weak_ptr : 가리키는 곳에 대한 소유권을 주장하지 않으며, 단순히 객체를 하나씩 가리키거나 관리하기 위해 쓴다.
//	 ㄴ 임시로 가리키기 위한 포인터이며, 참조 카운트를 증가시키지 않는다.

class Character
{
public:
	void Func1() {}
};

class Image
{
	// 그림 파일
};

class Monster
{
	std::shared_ptr<Image> mPic;

public:
	Monster(std::shared_ptr<Image> image)
	{
		mPic = image;
	}
};


int main()
{
	// 이 포인터는 중괄호가 끝나면 변수의 수명이 다하므로 해제된다.
	std::unique_ptr<Character> pChar{ new Character() };
	pChar->Func1();
	
	// unique_ptr 자체도 하나의 타입이므로, 일반 포인터와 달리 고유 기능이 있다.
	// . 을 찍으면 고유 기능에 접근할 수 있다. 예를 들어 release()를 하면 강제로 메모리를 해제한다.
	pChar.release();

	//-----------------------------------------------------------------------------------

	// 쉐어드 포인터를 생성할 때, 쉐어드 포인터로 만드려면 make_shared를 사용한다.
	// 유니크 포인터도 make_unique와 같이 만들 수 있다.
	std::shared_ptr<Image> pImage = std::make_shared<Image>();
	// 이렇게 선언함으로써 레퍼런스 카운터가 1 증가한다.

	//다만 모던 C++에서는 유니폼 초기화로 간단하게 표기 가능하다.
	std::shared_ptr<Image> pImage2{ new Image };

	{
		std::unique_ptr<Monster> m1{ new Monster(pImage) };
		// 레퍼런스 카운트 2
		{
			std::unique_ptr<Monster> m2{ new Monster(pImage) };
			// 레퍼런스 카운트 3
			{
				std::unique_ptr<Monster> m3{ new Monster(pImage) };
				// 레퍼런스 카운트 4
			}
			// 중괄호가 끝났으므로 m3 메모리 해제. 레퍼런스 카운트 3
		}
		// 레퍼런스 카운트 2
	}
	// 레퍼런스 카운트 1
	// main 함수가 끝날 때 레퍼런스 카운트가 0이 되고 메모리가 해제된다


	std::unique_ptr<Monster> m1{ new Monster(pImage2) };
	delete m1.get(); //참고로 스마트 포인터를 일반 포인터처럼 가져다 쓸 경우, .get()을 통해 가져와야 한다.
}