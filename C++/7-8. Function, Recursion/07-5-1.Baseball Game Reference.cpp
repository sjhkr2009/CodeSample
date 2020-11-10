#include <iostream>
#include <ctime>

// 숫자 야구 게임

//사용자 입력과 정답을 넣을 배열. 한 자리씩 비교해야 하므로 배열로 생성한다.
int gAnswer[3]{};
int gInput[3]{};

//임의의 숫자 생성
void CreateAnswer();
//사용자에게 입력 받기
void InputNumber();
//숫자 확인, 볼/스트라이크 출력, 정답인 경우 true 반환
bool CheckNumber();

int main()
{
	CreateAnswer();

	while (true)
	{
		InputNumber();

		if (CheckNumber())
		{
			break;
		}
	}
}

void CreateAnswer()
{
	// 시간을 시드로 넘겨 난수가 제대로 출력되도록 한다.
	std::srand(std::time(nullptr));

	// 이미 사용한 숫자는 중복되지 않도록 체크하기 위한 배열.
	bool useDigit[10]{};

	// Tip. 만일 배열이 아니라 그냥 3자리 정수를 랜덤으로 만들어야 한다면, 나온 숫자를 900으로 나눈 나머지에 100을 더하면 된다.

	for (int i = 0; i < 3; ++i)
	{
		int randomDigit{};
		while (true)
		{
			randomDigit = std::rand() % 10;

			if (i == 0 && randomDigit == 0)
			{
				continue;
			}

			if (!useDigit[randomDigit])
			{
				useDigit[randomDigit] = true;
				break;
			}
		}
		gAnswer[i] = randomDigit;
	}

}

void InputNumber()
{
	char input[4]{};

	std::cout << "세 자리 숫자를 입력하세요 : ";
	std::cin >> input;

	for (int i = 0; i < 3; ++i)
	{
		gInput[i] = input[i] - '0';
	}
}

bool CheckNumber()
{
	int ball{}, strike{};

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (gInput[i] == gAnswer[j])
			{
				if (i == j)
				{
					strike++;
				}
				else
				{
					ball++;
				}
			}
		}
	}

	if (strike == 3)
	{
		std::cout << "정답입니다!" << std::endl;
		return true;
	}

	std::cout << strike << " 스트라이크, " << ball << " 볼!" << std::endl;
	
	return false;
}
