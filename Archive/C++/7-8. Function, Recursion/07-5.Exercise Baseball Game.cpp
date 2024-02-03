#include <iostream>
#include <ctime>

// 숫자 야구 게임 만들기

const int DIGITS{ 3 }; // 숫자의 자릿수 (2 ~ 5까지 작동 확인. 단, rand의 최대값으로 인해 5자리는 큰 수가 잘 나오지 않음)

bool IsValid(int number);
bool IsCorrect(int input, int answer);
int DigitsOf(int number, int index);
int StrikeCheck(int input, int answer);
int BallCheck(int input, int answer);
void PrintResult(int ball, int strike);

int main()
{
	// rand에 시드로 현재시간을 값으로 넘겨 매번 다른 랜덤값을 산출한다.
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int answer{};

	// 정답에 랜덤값을 넣고, 원하는 자릿수만큼 자른다. 유효한 숫자가 나올 때까지 반복한다.
	do
	{
		answer = rand();
		answer %= static_cast<int>(pow(10, DIGITS));
	} while (!IsValid(answer));

	while (true)
	{
		int input{};
		std::cout << DIGITS << "자리 숫자를 입력하세요 : ";
		std::cin >> input;

		// [예외 처리] 지정된 자릿수의 숫자가 아닌 경우 에러 메시지를 띄우고 다시 입력을 받는다.
		if (input < static_cast<int>(pow(10, DIGITS - 1)) || input > static_cast<int>(pow(10, DIGITS) - 1))
		{
			std::cout << DIGITS << " 자리 정수를 입력해야 합니다." << std::endl;
			continue;
		}

		// 정답을 입력한 경우 게임을 종료한다.
		if (IsCorrect(input, answer))
		{
			std::cout << "정답입니다!" << std::endl;
			break;
		}

		// 정답이 아닌 경우 스트라이크와 볼을 체크하여 메시지를 띄운다.
		int strikeCount{ StrikeCheck(input, answer) };
		int ballCount{ BallCheck(input, answer) };

		PrintResult(ballCount, strikeCount);
	}

}

/// <summary>
/// 설정된 정답이 유효한 값인지 체크합니다. 자릿수가 다르거나 중복된 숫자가 있으면 false를 반환합니다.
/// </summary>
bool IsValid(int number)
{
	if (number < static_cast<int>(pow(10, DIGITS - 1)))
	{
		return false;
	}

	for (int i = 0; i < DIGITS; i++)
	{
		int now = DigitsOf(number, i);

		for (int j = i + 1; j < DIGITS; j++)
		{
			if (now == DigitsOf(number, j))
			{
				return false;
			}
		}
	}
	return true;
}

/// <summary>
/// 입력한 숫자가 정답과 일치하는지 체크합니다. 일치하면 게임이 종료됩니다.
/// </summary>
bool IsCorrect(int input, int answer)
{
	return (input == answer);
}

/// <summary>
/// n자리 숫자와 자릿수를 입력받아, 해당 자릿수의 숫자를 반환합니다.
/// </summary>
/// <param name="index">몇 번째 자리인지 입력합니다. 1, 10, 100, ...의 자리는 각각 0, 1, 2, ... 순으로 대응되며, 백의 자리를 구하려면 2를 입력해야 합니다.</param>
int DigitsOf(int number, int index)
{
	int result{};
	index = static_cast<int>(pow(10, index));
	result = number / index;
	return result % 10;
}

/// <summary>
/// 스트라이크 개수를 반환합니다. 각 자리수를 확인하여 같으면 카운트가 1 증가합니다.
/// </summary>
int StrikeCheck(int input, int answer)
{
	int count{ 0 };

	for (int i = 0; i < DIGITS; i++)
	{
		if (DigitsOf(input, i) == DigitsOf(answer, i))
		{
			count++;
		}
	}

	return count;
}

/// <summary>
/// 볼 개수를 반환합니다. 입력한 각 자리의 숫자가 정답 숫자에 포함되어 있으면 카운트가 1 증가합니다. 단, 숫자와 자릿수가 모두 일치하면 스트라이크이므로 여기서는 처리하지 않습니다.
/// </summary>
int BallCheck(int input, int answer)
{
	int count{ 0 };

	for (int i = 0; i < DIGITS; i++)
	{
		int currentNumber{ DigitsOf(input, i) };

		for (int j = 0; j < DIGITS; j++)
		{
			if (i == j)
			{
				continue;
			}
			if (currentNumber == DigitsOf(answer, j))
			{
				count++;
			}
		}
	}

	return count;
}

/// <summary>
/// 스트라이크와 볼의 개수에 따라 화면에 결과를 출력합니다. 0인 쪽은 출력하지 않으며, 둘 다 0이면 아웃을 출력합니다.
/// </summary>
void PrintResult(int ball, int strike)
{
	if (ball == 0 && strike == 0)
	{
		std::cout << "아웃입니다." << std::endl;
		return;
	}
	if (strike > 0)
	{
		std::cout << strike << "스트라이크";
		if (ball > 0)
		{
			std::cout << ", ";
		}
	}
	if (ball > 0)
	{
		std::cout << ball << "볼";
	}
	std::cout << "입니다." << std::endl;
}