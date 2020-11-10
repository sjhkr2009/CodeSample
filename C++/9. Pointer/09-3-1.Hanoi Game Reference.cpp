#include <iostream>

// 하노이의 탑 게임화

// 타워 1개에 해당하는 구조체를 생성한다.
// 기둥의 각 층(원반이 들어갈 공간)에 대한 정보와, 현재 가장 위의 원반이 존재하는 인덱스 값을 가지고 있다.
struct Tower
{
	int disk[3];
	int topIndex;
};

// 타워 개수가 3개이므로, 3개의 기둥을 만들어 값을 초기화한다.
// 첫 타워는 1,2,3번 원반이 차례로 들어있고, 나머지 타워는 비어있다. 빈 타워의 원반 인덱스는 (0~2번째 인덱스가 모두 비어 있으므로) 3으로 저장한다. 
Tower hanoi[3]
{
	{{1,2,3}, 0},
	{{0,0,0}, 3},
	{{0,0,0}, 3}
};

void PrintTowers();
bool MoveDisk(char from, char to);
bool IsFinish();

int main()
{
	char from{}, to{};
	while (true)
	{
		PrintTowers();

		std::cout << "어떻게 움직일까요? >> ";
		std::cin >> from >> to;

		if (!MoveDisk(from, to))
		{
			std::cout << "이동할 수 없습니다." << std::endl;
			continue;
		}

		if (IsFinish())
		{
			std::cout << "ㅊㅋ" << std::endl;
			break;
		}
	}
}

void PrintTowers()
{
	for (int height = 0; height < 3; height++)
	{
		for (int towerIndex = 0; towerIndex < 3; towerIndex++)
		{
			std::cout << hanoi[towerIndex].disk[height] << ' ';
		}
		std::cout << std::endl;
	}
}

bool MoveDisk(char from, char to)
{
	int fromColumnIndex{ from - '1' };
	int toColumnIndex{ to - '1' };

	// 기둥이 배열을 벗어나거나, 두 기둥이 같으면 이동 실패
	if (fromColumnIndex < 0 || fromColumnIndex >= 3 || toColumnIndex < 0 || toColumnIndex >= 3 || fromColumnIndex == toColumnIndex)
	{
		return false;
	}

	// from, to 각 기둥에서 맨 위의 원반이 있는 위치와, 현재 원반의 크기를 저장해둔다.
	int fromDiskIndex = hanoi[fromColumnIndex].topIndex;
	int toDiskIndex = hanoi[toColumnIndex].topIndex;
	int currentDisk = hanoi[fromColumnIndex].disk[fromDiskIndex];

	// 원반을 꺼내려는 기둥에 원반이 없거나, (그럴 일은 없지만) 원반을 옮기려는 기둥에 빈 자리가 없으면 이동 실패
	if (fromDiskIndex == 3 || toDiskIndex == 0)
	{
		return false;
	}

	// 목표 기둥에 원반이 있는데, 그 원반이 옮기려는 원반보다 작다면 이동 실패
	if (toDiskIndex != 3 && currentDisk > hanoi[toColumnIndex].disk[toDiskIndex])
	{
		return false;
	}

	//원반 이동 로직
	//	1) from 기둥에서 원반 꺼내기
	hanoi[fromColumnIndex].disk[fromDiskIndex] = 0;
	hanoi[fromColumnIndex].topIndex++;

	//	2) to 기둥에 원반 놓기
	hanoi[toColumnIndex].disk[toDiskIndex - 1] = currentDisk;
	hanoi[toColumnIndex].topIndex--;

	return true;
}

bool IsFinish()
{
	return (hanoi[2].disk[0] == 1) ? true : false;
}
