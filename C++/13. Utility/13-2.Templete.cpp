#include <iostream>

// 일반화
// template 키워드를 통해 일반화된 타입을 생성한다. 중복되는 타입을 줄여서 코딩할 수 있다. (C#의 General 기능과 유사함)
// 내부적으로는 사용된 타입만큼 코드가 중복 생성되므로 성능상의 이점은 없다.

template <typename T>
void Swap(T& a, T& b) // Swap 함수를 int, float, double 등 타입별로 일일이 생성할 필요가 없어진다.
{
	T temp{ a };
	a = b;
	b = temp;
}

// 일반화는 함수뿐만 아니라, 클래스에도 적용할 수 있다.
// template<class T>로 선언하면 T의 타입은 class로 한정되며, class의 내부 기능들을 이용할 수 있다. 한정하지 않는다면 typename으로 입력한다. (C#의 where 키워드와 유사함)

class Character {};
class Warrior : public Character {};

class Monster {};

template<class T>
class LinkedList
{
	T* head{ nullptr };
	T* tail{ nullptr };
};

// [주의사항]
// template을 사용한 함수는 선언과 정의가 한 파일에 존재해야 한다. 이를 위해 보통 2가지 방법이 사용된다. (내부적으로는 둘 다 동일)
// 1. 헤더 파일에 정의까지 하는 방식
// 2. .hpp 파일을 생성하여 정의를 작성한 후, template을 사용한 헤더 파일 아래쪽에 #include "(파일명).hpp"로 가져온다.


int main()
{
	int a{ 1 }, b{ 2 };
	float c{ 1.f }, d{ 2.5f };

	//일반화 타입을 명시적으로 표기할 수도, 생략할 수도 있다.
	Swap<int>(a, b);
	Swap(c, d);

	std::cout << a << '\t' << b << std::endl;
	std::cout << c << '\t' << d << std::endl;

	// 클래스는 컴파일러가 타입을 유추할 수 없으므로, <> 안에 타입을 명시해줘야 한다.
	LinkedList<Character> party;
	LinkedList<Monster> monsters;

	LinkedList<int> temp;
	//이것도 문법상 문제는 없다. C++의 모든 타입(int 포함)은 내부적으로 클래스이기 때문이다.
}
