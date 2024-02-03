#include<iostream>

int main()
{
	// 테트리스 블록 회전

	const int MAX_INDEX = 3;

	int block[MAX_INDEX + 1][MAX_INDEX + 1] =
	{
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	int tempBlock[MAX_INDEX + 1][MAX_INDEX + 1]{};
	int input{ 0 };

	//블록 출력
	for (int y = 0; y <= MAX_INDEX; y++)
	{
		for (int x = 0; x <= MAX_INDEX; x++)
		{
			std::cout << block[y][x];
		}
		std::cout << std::endl;
	}

	while (true)
	{
		std::cout << "블록 회전하기 (왼쪽 회전은 1, 오른쪽 회전은 2, 종료는 -1): ";
		std::cin >> input;

		//현재 블록 임시저장 (이후 임시저장한 블록으로부터 회전한 값을 블록에 덮어쓰기)
		for (int y = 0; y <= MAX_INDEX; y++)
		{
			for (int x = 0; x <= MAX_INDEX; x++)
			{
				tempBlock[y][x] = block[y][x];
			}
		}

		//왼쪽으로 회전 : x,y값 바꾼 다음, 세로로 뒤집기
		if (input == 1)
		{
			for (int y = 0; y <= MAX_INDEX; y++)
			{
				for (int x = 0; x <= MAX_INDEX; x++)
				{
					block[y][x] = tempBlock[x][MAX_INDEX - y];
				}
			}
		}
		//오른쪽으로 회전 : x,y값 바꾼 다음, 가로로 뒤집기
		else if (input == 2)
		{
			for (int y = 0; y <= MAX_INDEX; y++)
			{
				for (int x = 0; x <= MAX_INDEX; x++)
				{
					block[y][x] = tempBlock[MAX_INDEX - x][y];
				}
			}
		}
		//종료
		else if (input == -1)
		{
			break;
		}
		//예외 처리
		else
		{
			continue;
		}

		//회전한 블록 출력
		for (int y = 0; y <= MAX_INDEX; y++)
		{
			for (int x = 0; x <= MAX_INDEX; x++)
			{
				std::cout << block[y][x];
			}
			std::cout << std::endl;
		}

	}
}