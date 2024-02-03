#include <iostream>

// 팩토리얼
int Factorial(int n);

int main()
{
	int n{};
	std::cout << "숫자를 입력하세요 : ";
	std::cin >> n;

	std::cout << n << "! = " << Factorial(n);
}

int Factorial(int n)
{
	if (n <= 1)
	{
		return 1;
	}
	else
	{
		return n * Factorial(n - 1);
	}
}
