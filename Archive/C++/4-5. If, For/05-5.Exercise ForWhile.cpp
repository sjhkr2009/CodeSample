#include<iostream>

int main()
{
	// 1.	1,2,3,4,5 한 줄씩 출력
	std::cout << "# Number 1" << std::endl;
	for (int i = 1; i <= 5; i++)
	{
		std::cout << i << std::endl;
	}

	// 2.	1부터 10까지 홀수 출력
	std::cout << std::endl << "# Number 2" << std::endl;
	for (int i = 1; i <= 10; i += 2)
	{
		std::cout << i;
	}
	std::cout << std::endl;

	// 3.	a부터 z까지 출력
	std::cout << std::endl << "# Number 3" << std::endl;
	for (char i = 'a'; i <= 'z'; i++)
	{
		std::cout << i;
	}
	std::cout << std::endl;

	// 4.	구구단 2단 출력
	int num{ 2 };
	std::cout << std::endl << "# Number 4" << std::endl;
	for (int i = 1; i <= 9; i++)
	{
		std::cout << num << " x " << i << " = " << num * i << std::endl;
	}

	// 5.	구구단 단수를 입력받아 출력
	std::cout << std::endl << "# Number 5" << std::endl;

	std::cout << "몇 단을 출력할까요? >> " << std::endl;
	//std::cin >> num;
	for (int i = 1; i <= 9; i++)
	{
		std::cout << num << " x " << i << " = " << num * i << std::endl;
	}

	// 6.	구구단 전체 출력 (1 ~ 9단)
	std::cout << std::endl << "# Number 6" << std::endl;
	for (int i = 2; i <= 9; i++)
	{
		std::cout << i << "단" << std::endl;
		for (int j = 1; j <= 9; j++)
		{
			std::cout << i << " x " << j << " = " << i * j << std::endl;
		}
		std::cout << std::endl;
	}

	// 7.	두 개의 주사위를 던졌을 때, 두 눈의 합이 6이 되는 경우의 수
	std::cout << std::endl << "# Number 7" << std::endl;

	const int diceCount{ 6 };
	for (int i = 1; i <= diceCount; i++)
	{
		for (int j = 1; j <= diceCount; j++)
		{
			if (i + j != 6)
			{
				continue;
			}
			std::cout << "주사위 결과: ( " << i << ", " << j << " )" << std::endl;
		}
	}

	// 8.	정답이 6인 up-down 게임 -> 8.1. 정답이 랜덤인 up-down 게임
	std::cout << std::endl << "# Number 8" << std::endl;

	int answer{ 6 };

	//8.1에서 추가
	int count{ 0 };
	std::cout << "난이도를 입력하세요 (2 ~ 100, 스킵하려면 0) : ";
	//std::cin >> count;
	if (count != 0)
	{
		answer = rand() % count;
	}

	while (count != 0)
	{
		int input{};
		std::cout << "숫자를 입력하세요 : ";
		std::cin >> input;

		if (input > answer)
		{
			std::cout << "더 작은 수를 입력하세요." << std::endl;
		}
		else if (input < answer)
		{
			std::cout << "더 큰 수를 입력하세요." << std::endl;
		}
		else
		{
			std::cout << "정답입니다." << std::endl;
			break;
		}
	}

	// 9.	높이와 너비를 입력받은 후 사각형 그리기
	std::cout << std::endl << "# Number 9" << std::endl;

	int width{ 3 };
	int height{ 4 };
	std::cout << "너비를 입력하세요: ";
	//std::cin >> width;
	std::cout << "높이를 입력하세요: ";
	//std::cin >> height;

	for (int i = 0; i < height; i++)
	{
		std::string line{ "" };
		for (int j = 0; j < width; j++)
		{
			line += "*";
		}
		std::cout << line << std::endl;
	}

	// 10.	1, 12, 123, ... 한 줄씩 출력 (string 형변환, pow() 사용하지 말 것)
	std::cout << std::endl << "# Number 10" << std::endl;

	int lineCount{ 5 };
	std::cout << "출력할 횟수를 입력하세요 (1 ~ 9): ";
	//std::cin >> lineCount;

	for (int i = 1; i <= lineCount; i++)
	{
		int writeLine{ 0 };
		int power{ 1 };
		for (int j = i; j >= 1; j--)
		{
			writeLine += (j * power);
			power *= 10;
		}
		std::cout << writeLine << std::endl;
	}
	
	// 11.	삼각형 그리기
	std::cout << std::endl << "# Number 11" << std::endl;

	height = 0;
	std::cout << "삼각형 높이를 입력하세요 : ";
	std::cin >> height;

	for (int i = 0; i < height; i++)
	{
		int space = height - i;
		std::string writeLine{ "" };

		for (int j = 0; j < space; j++)
		{
			writeLine += " ";
		}

		for (int j = 0; j <= i; j++)
		{
			writeLine += "*";
			if (j != i)
			{
				writeLine += " ";
			}
		}
		std::cout << writeLine << std::endl;
	}

}