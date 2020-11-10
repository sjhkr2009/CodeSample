#include <iostream>

int main()
{
	// 홀수 마방진 만들기 (5x5)

	const int COLUMN{ 5 };

	int square[COLUMN][COLUMN]{};

	int value{ 1 };
	int x{ COLUMN / 2 }, y{};
	int prevX{}, prevY{};

	square[y][x] = 1;

	while (value <= COLUMN * COLUMN)
	{
		prevX = x++;
		prevY = y--;
		
		if (y < 0)
		{
			y = COLUMN - 1;
		}
		if (x >= COLUMN)
		{
			x = 0;
		}

		if (square[y][x] != 0)
		{
			x = prevX;
			y = prevY + 1;
		}

		square[y][x] = ++value;
	}

	for (int y = 0; y < COLUMN; y++)
	{
		for (int x = 0; x < COLUMN; x++)
		{
			std::cout.width(2);
			std::cout.fill(' ');
			std::cout << square[y][x] << ' ';
		}
		std::cout << std::endl;
	}
}