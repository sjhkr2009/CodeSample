#include<iostream>

int main()
{
	std::cout << std::endl << "----- 1번 -----" << std::endl;
	// #1
	for (int i = 1; i <= 5; i++)
	{
		std::cout << i << std::endl;
	}

	std::cout << std::endl << "----- 2번 -----" << std::endl;
	// #2
	for (int i = 1; i <= 9; i += 2)
	{
		if (i == 5)
		{
			continue;
		}
		std::cout << i << std::endl;
	}

	std::cout << std::endl << "----- 3번 -----" << std::endl;
	// #3
	for (char i = 'a'; i <= 'z'; i++)
	{
		std::cout << i << ' ';
	}

	std::cout << std::endl << "----- 4번 -----" << std::endl;
	// #4
	int n{0};
	while (n < 1 || n > 9)
	{
		std::cout << "출력할 구구단의 단수를 입력하세요 : ";
		std::cin >> n;
	}
	for (int i = 1; i <= 9; i++)
	{
		std::cout << n << " x " << i << " = " << n * i << std::endl;
	}

	std::cout << std::endl << "----- 5번 -----" << std::endl;
	// #5
	for (int i = 2; i <= 9; i++)
	{
		std::cout << i << "단" << std::endl;
		for (int j = 1; j <= 9; j++)
		{
			std::cout << i << " x " << j << " = " << i * j << std::endl;
		}
	}

	std::cout << std::endl << "----- 6번 -----" << std::endl;
	// #6
	for (int i = 1; i <= 6; i++)
	{
		for (int j = 1; j <= 6; j++)
		{
			if (i + j == 6)
			{
				std::cout << "(" << i << ", " << j << ")" << std::endl;
			}
		}
	}

	std::cout << std::endl << "----- 7번 -----" << std::endl;
	// #7
	int correct(6);
	while (true)
	{
		int answer{};
		std::cout << "입력 : ";
		std::cin >> answer;

		if (answer > correct)
		{
			std::cout << "더 작은 수를 입력하세요." << std::endl;
		}
		else if (answer < correct)
		{
			std::cout << "더 큰 수를 입력하세요." << std::endl;
		}
		else
		{
			std::cout << "정답입니다." << std::endl;
			break;
		}
	}

	std::cout << std::endl << "----- 8번 -----" << std::endl;
	// #8
	int width{ 0 };
	int height{ 0 };

	while (width <= 0 || height <= 0)
	{
		std::cout << "너비를 입력하세요 : ";
		std::cin >> width;
		std::cout << "높이를 입력하세요 : ";
		std::cin >> height;
	}

	for (int i = 0; i < height; i++)
	{
		std::cout << std::endl;
		for (int j = 0; j < width; j++)
		{
			std::cout << "*";
		}
	}

	std::cout << std::endl << "----- 9번 -----" << std::endl;
	// #9
	for (int i = 1; i <= 5; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			std::cout << j;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << "----- 10번 -----" << std::endl;
	// #10
	int space{ -1 };
	height = 0;
	while (height <= 0)
	{
		std::cout << "삼각형의 높이를 입력하세요 : ";
		std::cin >> height;
	}
	std::cout << std::endl;
	for (int i = 1; i <= height; i++)
	{
		for (int j = 0; j < height - i; j++)
		{
			std::cout << " ";
		}
		for (int k = 0; k < i; k++)
		{
			std::cout << "* ";
		}
		std::cout << std::endl;
	}

}