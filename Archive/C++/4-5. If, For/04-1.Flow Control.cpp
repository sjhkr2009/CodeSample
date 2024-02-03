#include<iostream>

int main()
{
	int inputNum{};

	std::cout << "숫자를 입력하세요 : ";
	std::cin >> inputNum;

	inputNum = inputNum >= 0 ? inputNum : -inputNum;

	std::cout << "입력하신 숫자의 절대값은 " << inputNum << " 입니다.";
}