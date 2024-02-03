#include <iostream>

// 구조체 포인터로 구조체 내의 변수 불러오기

struct Stat 
{
	int hp;
	int mp;
};

int main()
{
	Stat player1{ 100, 10 };
	Stat* pPlayer1{ nullptr };

	// 포인터가 player1을 가리키게 한다.
	pPlayer1 = &player1;

	// * 보다 . 의 우선순위가 높으므로, 구조체 내의 값을 출력하려면 포인터에 괄호를 붙여 (*pPlayer1)를 만든 다음, 그곳의 값을 불러와야 한다.
	// 괄호가 없으면 *(pPlayer1.hp) 와 같이 연산해버려서 에러가 난다.
	std::cout << (*pPlayer1).hp << std::endl;

	// 이를 간소화하면 다음과 같다. ( 구조체 포인터 -> 변수 )
	std::cout << pPlayer1->hp << std::endl;
}