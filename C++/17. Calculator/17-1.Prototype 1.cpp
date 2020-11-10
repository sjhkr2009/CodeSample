#include <iostream>

// 계산기 만들기

// 프로토타입 1: 덧셈과 뺄셈

int main()
{
	int a, b, result{};
	char oper{};

	std::cout << "계산식을 입력하세요." << std::endl <<
		"> ";
	std::cin >> a >> oper >> b;

	switch (oper)
	{
		case '+':
			result = a + b;
			break;
		case '-':
			result = a - b;
			break;
		default:
			std::cout << "잘못된 수식입니다." << std::endl;
			return -1;
	}
	
	std::cout << "결과: " << result << std::endl;

	return 0;
}
