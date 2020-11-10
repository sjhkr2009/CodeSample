#include <iostream>

// enum class를 쓰면, 해당 enum 안의 값은 범위 지정 연산자 ::를 사용해서 접근해야 한다.

enum class Flags
{
	Quest1,
	Quest2
};


int main()
{
	Flags quest = Flags::Quest1;

	switch (quest)
	{
		case Flags::Quest1:
			break;
		case Flags::Quest2:
			break;
		default:
			break;
	}
}