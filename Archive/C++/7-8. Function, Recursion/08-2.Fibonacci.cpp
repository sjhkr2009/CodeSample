#include <iostream>

// 피보나치 수열 (재귀함수로 만들기)
int Fibonacci(int n);

int main()
{
	int n{};
	std::cout << "숫자를 입력하세요 : ";
	std::cin >> n;

	std::cout << n << "이하의 피보나치 수열" << std::endl;
	int count{ (n > 6) ? n : 6 };
	for (int i = 0; i < count; i++)
	{
		int fibo = Fibonacci(i);
		if (fibo > n)
		{
			break;
		}
		std::cout << fibo << ' ';
	}
}

int Fibonacci(int n)
{
	if (n <= 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
	{
		return Fibonacci(n - 1) + Fibonacci(n - 2);
	}
}