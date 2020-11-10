#include <iostream>

// 하노이의 탑
void HanoiInit(int height, int& moveCount);
void Hanoi(int n, char from, char temp, char to, int& moveCount);
void Move(int n, char from, char to, int& moveCount);

int main()
{
	int height{}, moveCount{};
	std::cout << "원반 개수를 입력하세요 : ";
	std::cin >> height;

	HanoiInit(height, moveCount);
	std::cout << "이동 횟수: " << moveCount;
}

void HanoiInit(int height, int& moveCount)
{
	Hanoi(height, 'A', 'B', 'C', moveCount);
}

void Hanoi(int n, char from, char temp, char to, int& moveCount)
{
	if (n == 1)
	{
		Move(n, from, to, moveCount);
	}
	else
	{
		Hanoi(n - 1, from, to, temp, moveCount);
		Move(n, from, to, moveCount);
		Hanoi(n - 1, temp, from, to, moveCount);
	}
}

void Move(int n, char from, char to, int& moveCount)
{
	std::cout << ++moveCount << " : " << from << "에서 " << to << "로 원반 " << n << "을 옮깁니다." << std::endl;
}
