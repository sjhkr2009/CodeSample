#include <iostream>


int main()
{
	//테트리스 블록 회전

	int block[3][3] =
	{
		{0,1,0},
		{0,1,1},
		{0,1,0}
	};

	std::cout << "--- Play Tetris ---" << std::endl;
	std::cout << "1: 왼쪽 회전, 2: 오른쪽 회전" << std::endl;

	int rotate{};
	int newX{}, newY{};
	int rotBlock[3][3]{};

	while (true)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				std::cout << block[y][x] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << "회전 방향을 입력하세요: ";
		std::cin >> rotate;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (rotate == 1)
				{
					newX = y;
					newY = 2 - x;
				}
				else
				{
					newX = 2 - y;
					newY = x;
				}

				rotBlock[newY][newX] = block[y][x];
			}
		}

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				block[y][x] = rotBlock[y][x];
			}
		}
	}
}