#include<iostream>

int main()
{
	std::cout << "숫자를 입력한 후 엔터를 누르세요.\n";

	int number1 = -1;
	int number2 = -1;

	std::cin >> number1 >> number2;
	
	std::cout << "\n입력하신 숫자는 " << number1 << ", " << number2 << " 입니다.\n";
}