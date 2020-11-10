#include<iostream>

int main()
{
	// 지하철 요금 계산
	// 이용거리, 교통카드/일반, 어른/청소년/어린이 구분에 따른 요금 산출하기
	// 어른: 19세 이상 / 청소년: 13세~18세 / 어린이: 6세~12세

	// 입력: 나이, 결제수단, 이동거리

	//기본요금
	const int CHILD_PRICE{ 400 };
	const int YOUTH_CARD_PRICE{ 640 };
	const int ADULT_CARD_PRICE{ 1150 };
	const int NOTCHILD_MONEY_PRICE{ 1250 };

	//단위 거리에 따른 요금 증가량
	const int CHILD_FACTOR{ 50 };
	const int YOUTH_FACTOR{ 80 };
	const int ADULT_FACTOR{ 100 };

	int age{};
	int payment{};
	int distance{};

	int price{ 0 };
	int distanceFactor{};

	std::cout << "나이를 입력하세요 : ";
	std::cin >> age;
	std::cout << "결제수단을 입력하세요. (교통카드는 0, 일반은 1) : ";
	std::cin >> payment;
	std::cout << "이동거리를 입력하세요(km) : ";
	std::cin >> distance;
	
	distanceFactor = distance < 5 ? 1 : (distance / 5);
	if (distance >= 10 && (distance % 5) == 0)
	{
		distanceFactor--;
	}

	if (age < 6)
	{
		price = 0;
	}
	else if (age < 13)
	{
		price = CHILD_PRICE + (distanceFactor * CHILD_FACTOR);
	}
	else if (age < 19 && payment == 0)
	{
		price = YOUTH_CARD_PRICE + (distanceFactor * YOUTH_FACTOR);
	}
	else if (payment == 0)
	{
		price = ADULT_CARD_PRICE + (distanceFactor * ADULT_FACTOR);
	}
	else
	{
		price = NOTCHILD_MONEY_PRICE + (distanceFactor * ADULT_FACTOR);
	}

	
//#pragma region 공식 미적용
//
//	if (payment == 0)
//	{
//		if (age < 6)
//		{
//			price = 0;
//		}
//		else if (age < 13)
//		{
//			if (distance <= 10)
//			{
//				price = 450;
//			}
//			else if (distance <= 15)
//			{
//				price = 500;
//			}
//			else if (distance <= 20)
//			{
//				price = 550;
//			}
//		}
//		else if (age < 19)
//		{
//			if (distance <= 10)
//			{
//				price = 720;
//			}
//			else if (distance <= 15)
//			{
//				price = 800;
//			}
//			else if (distance <= 20)
//			{
//				price = 880;
//			}
//		}
//		else
//		{
//			if (distance <= 10)
//			{
//				price = 1250;
//			}
//			else if (distance <= 15)
//			{
//				price = 1350;
//			}
//			else if (distance <= 20)
//			{
//				price = 1450;
//			}
//		}
//	}
//	else if (payment == 1)
//	{
//		if (age < 6)
//		{
//			price = 0;
//		}
//		else if (age < 13)
//		{
//			if (distance <= 10)
//			{
//				price = 450;
//			}
//			else if (distance <= 15)
//			{
//				price = 500;
//			}
//			else if (distance <= 20)
//			{
//				price = 550;
//			}
//		}
//		else
//		{
//			if (distance <= 10)
//			{
//				price = 1350;
//			}
//			else if (distance <= 15)
//			{
//				price = 1450;
//			}
//			else if (distance <= 20)
//			{
//				price = 1550;
//			}
//		}
//	}
//
//#pragma endregion

	std::cout << "가격 : " << price << "원";

}