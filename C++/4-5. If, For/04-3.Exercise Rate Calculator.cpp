#include<iostream>

int main()
{
	//	1. 달러(USD)를 입력받아 원(KRW)으로 변환하기
	//	2. 달러(USD) 또는 엔화(JPY)를 원(KRW)으로 전환하기 (단, 화폐단위는 입력 숫자 뒤에 S(달러), Y(엔)을 붙이기로 한다)
	//		ㄴ 입력 예시: 10S, 75Y


	const float USD_TO_KRW{ 1196.f };
	const float JPY_TO_KRW{ 11.15f };

	// 1번

	int inputUSD{};
	int outputKRW{};

	std::cout << "달러(USD)를 입력하세요 : ";
	std::cin >> inputUSD;

	outputKRW = inputUSD * USD_TO_KRW;

	std::cout << inputUSD << " 달러는 " << outputKRW << " 원 입니다." << std::endl;

	// 2번

	int inputCurrency{};
	char currencyType{};

	std::cout << "환율을 계산한 달러(USD) 또는 엔(JPY)을 입력하세요." << std::endl <<
		"달러는 금액 뒤에 S, 엔화는 Y를 붙여야 합니다. (예: 10S, 75Y 등)" << std::endl;
	std::cin >> inputCurrency >> currencyType;

	bool isDoller = (currencyType == 'S');

	outputKRW = isDoller ? (inputCurrency * USD_TO_KRW) : (inputCurrency * JPY_TO_KRW);
	std::string outputType{ isDoller ? "달러" : "엔" };

	std::cout << inputCurrency << outputType <<
		"은(는) 한화로 " << outputKRW << "원 입니다." << std::endl;
}