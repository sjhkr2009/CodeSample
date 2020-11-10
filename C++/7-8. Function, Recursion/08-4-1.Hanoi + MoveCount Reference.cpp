#include <iostream>

// 하노이의 탑 이동 횟수 표기

int moveCount{ 1 };

void Move(int index, char from, char to)
{
	std::cout << moveCount++ << " : " <<
		from << " 에서 " << to << " 로 " <<
		index << "번 원반을 이동합니다." << std::endl;
}

void Hanoi(int n, char from, char temp, char to)
{
	if (n == 1)
	{
		Move(n, from, to);
	}
	else
	{
		Hanoi(n - 1, from, to, temp);
		Move(n, from, to);
		Hanoi(n - 1, temp, from, to);
	}
}

int main()
{
	Hanoi(4, 'A', 'B', 'C');
}