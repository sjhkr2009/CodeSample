#include<iostream>

int main()
{
	// char 문자열 Exercise #1
	// (string 및 관련 메서드 사용 X)
	
	// #1. 거꾸로 출력하기
	
	char inputA[10]{};
	int stringCount{ 0 };

	char outputA[10]{};

	std::cout << "문자열을 입력하세요: ";
	std::cin >> inputA;

	for (int i = 0; i < 10; i++)
	{
		if (inputA[i] == '\0')
		{
			stringCount = i;
			break;
		}
	}

	for (int i = 0; i < stringCount; i++)
	{
		outputA[(stringCount - i) - 1] = inputA[i];
	}

	std::cout << "문자열 뒤집기 결과 : " << outputA << std::endl;


	//-------------------------------------------------------------------------
	// #2. 두 문자열 합쳐서 출력하기

	char inputB[10]{};
	char inputC[10]{};
	stringCount = 0;

	char outputB[20]{};

	std::cout << "첫 번째 문자열을 입력하세요: ";
	std::cin >> inputB;
	std::cout << "두 번째 문자열을 입력하세요: ";
	std::cin >> inputC;

	for (int i = 0; i <= 10; i++)
	{
		if (i >= 10 || inputB[i] == '\0')
		{
			stringCount = i;
			break;
		}
		else
		{
			outputB[i] = inputB[i];
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (i >= 10 || inputC[i] == '\0')
		{
			break;
		}
		else
		{
			outputB[stringCount + i] = inputC[i];
		}
	}

	std::cout << "합쳐진 문자열: " << outputB << std::endl;


	//-------------------------------------------------------------------------
	// #3. 대문자로 변경

	int toBigger{ (int)('A' - 'a') };

	char inputD[10]{};
	stringCount = 0;

	char outputC[10]{};
	
	std::cout << "대문자로 변경할 문자열을 입력하세요 (영어 소문자) : ";
	std::cin >> inputD;

	for (int i = 0; i < 10; i++)
	{
		if (inputD[i] == '\0')
		{
			stringCount = i;
			break;
		}
	}

	for (int i = 0; i < stringCount; i++)
	{
		outputC[i] = inputD[i] + toBigger;
	}

	std::cout << "대문자 변환: " << outputC << std::endl;
}