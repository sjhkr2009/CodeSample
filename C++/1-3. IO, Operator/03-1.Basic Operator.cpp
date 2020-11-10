#include<iostream>

int main()
{
	int a{};
	int b{};

	do
	{
		std::cout << "두 개의 정수를 입력하세요. (b≠0)" << std::endl;

		std::cin >> a >> b;

	} while (b == 0);

	std::cout << "a + b = " << a + b << std::endl
		<< "a - b = " << a - b << std::endl
		<< "a * b = " << a * b << std::endl
		<< "a / b = " << a / b << std::endl
		<< "a % b = " << a % b << std::endl;
}