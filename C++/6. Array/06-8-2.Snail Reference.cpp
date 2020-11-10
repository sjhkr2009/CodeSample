#include <iostream>


int main()
{
	//달팽이 만들기 (5x5)

	enum Direction
	{
		오른쪽,
		아래,
		왼쪽,
		위
	};

	const int COLUMN{ 5 };
	int snail[COLUMN][COLUMN]{};
	Direction dir{ 오른쪽 };

	int x{}, y{};
	int value{ 1 };

	while (value <= COLUMN * COLUMN)
	{
		snail[y][x] = value++;

		switch (dir)
		{
			case 오른쪽:
				x++;
				if (x == COLUMN || snail[y][x] != 0)
				{
					dir = 아래;
					x--;
					y++;
				}
				break;

			case 아래:
				y++;
				if (y == COLUMN || snail[y][x] != 0)
				{
					dir = 왼쪽;
					y--;
					x--;
				}
				break;

			case 왼쪽:
				x--;
				if (x < 0 || snail[y][x] != 0)
				{
					dir = 위;
					x++;
					y--;
				}
				break;

			case 위:
				y--;
				if (y < 0 || snail[y][x] != 0)
				{
					dir = 오른쪽;
					y++;
					x++;
				}
				break;

			default:
				break;
		}
	}

	for (int y = 0; y < COLUMN; y++)
	{
		for (int x = 0; x < COLUMN; x++)
		{
			std::cout.width(2); // 숫자 하나에 2칸씩 쓰겠다는 의미
			std::cout.fill(' ');// 빈 자리가 있다면 공백(' ')으로 채운다는 의미

			std::cout << snail[y][x] << ' ';
		}
		std::cout << std::endl;
	}
}