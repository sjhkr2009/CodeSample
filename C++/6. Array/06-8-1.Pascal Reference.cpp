#include <iostream>


int main()
{
	//파스칼의 삼각형 5열 출력
	
	const int COLUMN{ 5 };

	int triangle[COLUMN][COLUMN]{};

	for (int i = 0; i < COLUMN; i++)
	{
		if (i == 0)
		{
			triangle[0][0] = 1;
			continue;
		}

		int prevValue{};

		for (int j = 0; j <= i; j++)
		{
			prevValue = (j - 1) < 0 ? 0 : triangle[i - 1][j - 1];

			triangle[i][j] = prevValue + triangle[i - 1][j];
		}
	}

	for (int i = 0; i < COLUMN; i++)
	{
		for (int j = 0; j < COLUMN - i; j++)
		{
			std::cout << ' ';
		}
		
		for (int j = 0; j < COLUMN; j++)
		{
			if (triangle[i][j] != 0)
			{
				std::cout << triangle[i][j] << ' ';
			}
		}
		std::cout << std::endl;
	}
}