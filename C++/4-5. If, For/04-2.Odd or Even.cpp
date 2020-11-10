#include<iostream>

int main()
{
	int inputNum{};

	std::cout << "숫자를 입력하세요 : ";
	std::cin >> inputNum;

	std::string result{ (inputNum % 2) == 0 ? "짝수" : "홀수" };

	std::cout << "입력한 숫자는 " << result << " 입니다." << std::endl;
}