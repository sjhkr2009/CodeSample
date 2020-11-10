#include<iostream>

int main()
{
	// 세계 최초의 프로그램 (1949)
	// : 0부터 99까지의 제곱을 계산하는 프로그램

	int x{ 0 };

	while (x < 100)
	{
		std::cout << x << "의 제곱: " << x * x << std::endl;
		++x;
	}
}