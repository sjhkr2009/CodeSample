#include<iostream>

int main()
{
	// 달러, 엔, 유로, 또는 파운드를 입력받아 한화로 변환하기
	// 달러는 뒤에 S, 엔은 Y, 유로는 E, 파운드는 P가 붙는다.

	const float USD_TO_KRW{ 1197.f };
	const float JPY_TO_KRW{ 11.19f };
	const float EUR_TO_KRW{ 1385.23f };
	const float GBP_TO_KRW{ 1518.04f };

	int inputCurrency{};
	char type{};
	float rate{};

	std::cout << "환율을 계산할 외화를 입력하세요. (달러는 S, 엔은 Y, 유로는 E, 파운드는 P가 붙어야 합니다)" << std::endl << " >> ";
	std::cin >> inputCurrency >> type;

	std::string outputType{};
	int changedCurrency{};

	switch (type)
	{
		case 'S':case's':
			rate = USD_TO_KRW;
			outputType = "달러";
			break;
		case 'Y':case'y':
			rate = JPY_TO_KRW;
			outputType = "엔";
			break;
		case 'E':case'e':
			rate = EUR_TO_KRW;
			outputType = "유로";
			break;
		case 'P':case'p':
			rate = GBP_TO_KRW;
			outputType = "파운드";
			break;
		default:
			std::cout << "올바른 입력이 아닙니다.";
			return -1;
	}
	changedCurrency = (int)(inputCurrency * rate);

	std::cout << inputCurrency << outputType << 
		" = " << changedCurrency << "원 입니다." << std::endl;
}