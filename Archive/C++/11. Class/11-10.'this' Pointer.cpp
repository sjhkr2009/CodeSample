#include <iostream>

// *this의 활용

class Character1
{
public:
	void Move() { std::cout << "이동" << " "; };
	void TurnLeft() { std::cout << "좌회전" << " "; };
	void TurnRight() { std::cout << "우회전" << " "; };
	void Swing() { std::cout << "공격" << " "; };
};

class Character2
{
public:
	Character2& Move() { return *this; };
	Character2& TurnLeft() { return *this; };
	Character2& TurnRight() { return *this; };
	Character2& Swing() { return *this; };
};

int main()
{
	Character1 player1;

	player1.Move();
	player1.TurnLeft();
	player1.TurnRight();
	player1.Swing();

	//void 반환형을 자기 자신의 참조형을 반환하는 것으로 수정하여 해당 연속 동작을 간단하게 나타낼 수 있다.

	Character2 player2;

	player2.Move().TurnRight().Swing().TurnRight();
}