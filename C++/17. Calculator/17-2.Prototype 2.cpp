#include <iostream>

// 계산기 만들기

// 프로토타입 2: 사칙연산과 반복

int main()
{
	int lValue{}, rValue{};
	char oper{};

	std::cout << "계산식을 입력한 후 = 을 입력해주세요 (예: 2 + 5 - 1 = )." << std::endl <<
		"> ";
	// 첫 번째 숫자를 lValue에 넣는다.
	std::cin >> lValue;

	// 계속 연산자를 받아오며 반복한다.
	for (char oper; std::cin >> oper;)
	{
		// 계산이 끝나지 않았다면, 다음 숫자를 rValue에 받아온다.
		if (oper != '=')
		{
			std::cin >> rValue;
		}
		// [예외 처리] 입력에 오류가 있으면(받아올 숫자가 없을 경우) 에러 메시지 출력
		if (!std::cin)
		{
			throw std::runtime_error("피연산자가 없습니다.");
		}

		switch (oper)
		{
			case '+':
				lValue += rValue;
				break;
			case '-':
				lValue -= rValue;
				break;
			case '*':
				lValue *= rValue;
				break;
			case '/':
				lValue /= rValue;
				break;
			// 연산자가 +,-,*,/ 중 하나가 아닐 경우 더 이상 계산할 수 없으므로, 결과를 출력하고 종료한다.
			default:
				std::cout << "결과: " << lValue << std::endl;
				return 0;
		}
	}

	// 첫 번째 값이 잘못 입력된 경우 for문으로 진입하지 않고 여기로 넘어온다.
	std::cerr << "잘못된 수식입니다." << std::endl;
	return 1;
}
