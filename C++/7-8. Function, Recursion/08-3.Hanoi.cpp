#include <iostream>

// 하노이의 탑
void HanoiInit(int height);
void Hanoi(int n, char from, char temp, char to);
void Move(char from, char to);

int main()
{
	int n{};
	std::cout << "원반 개수를 입력하세요 : ";
	std::cin >> n;
	
	HanoiInit(n);
}

void HanoiInit(int height)
{
	Hanoi(height, 'A', 'B', 'C');
}

void Hanoi(int n, char from, char temp, char to)
{
	if (n == 1)
	{
		Move(from, to);
	}
	else
	{
		Hanoi(n - 1, from, to, temp);
		Move(from, to);
		Hanoi(n - 1, temp, from, to);
	}
}

void Move(char from, char to)
{
	std::cout << from << " → " << to << " 으로 이동" << std::endl;
}
