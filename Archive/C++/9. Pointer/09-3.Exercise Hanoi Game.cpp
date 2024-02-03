#include <iostream>

// 하노이의 탑 - 게임으로 만들기

int* SetColumn(int height, int index);
void PrintColumn(int* columnA, int* columnB, int* columnC, int count);
void Move(int* from, int* to, int& moveCount, int height);
bool IsCompleted(int* lastColumn);

int main()
{
	// 원반의 개수를 입력받고, 이동 횟수를 측정하기 위한 moveCount 변수를 선언한다.
	int height{}, moveCount{};
	std::cout << "원반 개수를 입력하세요 : ";
	std::cin >> height;

	// 기둥의 개수는 3개로 고정이므로, 각각의 기둥을 배열 포인터로 선언한다.
	int* columnA = SetColumn(height, 0);
	int* columnB = SetColumn(height, 1);
	int* columnC = SetColumn(height, 2);

	// 좀 더 용이하게 사용하기 위해 포인터들을 배열로 저장해 둔다.
	int* columns[3]{ columnA, columnB, columnC };

	// 생성된 기둥을 시각화하여 보여준다.
	PrintColumn(columnA, columnB, columnC, height);

	// 완료되지 않은 한 반복한다.
	while (!IsCompleted(columnC))
	{
		// 원반을 어디에서 어디로 이동할 지 기둥 번호로 받는다.
		int from{ 0 };
		int to{ 0 };
		std::cout << "어떻게 움직일까요? (예: 1 3) >> ";
		std::cin >> from >> to;

		// [예외 처리] 기둥 번호가 유효하지 않으면 다시 입력받는다.
		if (from < 1 || to < 1 || from > 3 || to > 3)
		{
			std::cout << "잘못된 입력입니다. 기둥의 번호를 1~3 사이의 숫자로 입력하세요." << std::endl;
			continue;
		}

		// 이동시키고 이동 후의 기둥 상태를 보여준다. 이동이 불가한 조건은 Move 함수에서 처리한다.
		Move(columns[from - 1], columns[to - 1], moveCount, height);
		PrintColumn(columnA, columnB, columnC, height);
	}

	std::cout << "축하합니다. 정답입니다." << std::endl;

	std::cout << "이동 횟수: " << moveCount;
}

/// <summary>
/// 기둥을 최초 생성하기 위한 함수. int 배열의 포인터를 반환하며 인덱스 0부터 height - 1까지 위에서 아래로 원반이 들어갈 자리가 생성되며, 마지막 인덱스에는 기둥 번호를 넣는다.
/// </summary>
/// <param name="height">원반의 개수. 배열의 크기를 결정한다.</param>
/// <param name="index">기둥의 번호. 배열의 마지막 요소로 입력된다. 입력받은 기둥 번호를 알기 위해 활용되며, 플레이어에게 보여주지는 않는다. 첫 번째 기둥인 경우 원반을 채워 넣고, 아닌 경우 빈 기둥을 만들어 반환한다.</param>
int* SetColumn(int height, int index)
{
	int* p = new int[height + 1];
	for (int i = 0; i < height; i++)
	{
		*(p + i) = (index == 0) ? (i + 1) : 0;
	}
	*(p + height) = index;
	return p;
}

/// <summary>
/// 기둥을 출력하는 함수. 각각의 기둥을 매개변수로 받으며, count에는 기둥의 높이가 들어간다.
/// </summary>
void PrintColumn(int* columnA, int* columnB, int* columnC, int count)
{
	int* columns[3]{ columnA, columnB, columnC };

	for (int y = 0; y < count; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			std::cout << *(columns[x] + y) << '\t';
		}
		std::cout << std::endl;
	}
}

/// <summary>
/// 기둥에서 원반을 이동한다. 기둥의 가장 높은 곳의 원반이 이동되므로 별도로 원반 번호를 받을 필요는 없다.
/// </summary>
/// <param name="from">옮기려는 원반이 있는 기둥</param>
/// <param name="to">원반을 옮겨놓을 기둥</param>
/// <param name="moveCount">이동 횟수. 이동 성공 시 횟수를 증가시킨다. 출력에도 이용되므로 이 변수를 지우고 Move에서 int를 반환하게 하는 것은 권장되지 않는다.</param>
/// <param name="height">기둥의 높이. 즉, 원반의 개수.</param>
void Move(int* from, int* to, int& moveCount, int height)
{
	// [예외 처리] 두 기둥이 같으면 에러 메시지 출력 후 return
	if (from == to)
	{
		std::cout << "원반이 있는 기둥과 옮길 기둥은 서로 달라야 합니다." << std::endl;
		return;
	}
	
	
	// 대상 원반의 크기
	int target{};
	// from 배열에서 옮길 원반이 있는 위치. 위에서 3번째에 있다면 2를 대입한다. 기둥이 비어 있어서 탐색에 실패했다면 -1을 기본값으로 갖는다.
	int fromIndex{ -1 };
	// to 배열에서 옮길 원반이 들어갈 위치. 기둥이 비어 있어서 탐색에 실패했다면 (기둥 번호를 제외한) 가장 마지막 인덱스를 기본값으로 갖는다.
	int toIndex{ height - 1 };

	// from 배열에서 위에서부터 순차적으로 원반이 있는 칸을 찾는다. 원반이 있다면 해당 인덱스와 원반 크기를 저장한다.
	for (int i = 0; i < height; i++)
	{
		if (*(from + i) > 0)
		{
			fromIndex = i;
			target = *(from + i);
			break;
		}
	}

	// [예외 처리] from 기둥에 원반이 없어 기본값을 그대로 갖고 있다면 에러 메시지 출력 후 return
	if (fromIndex == -1)
	{
		std::cout << "지정한 기둥에 원반이 없습니다." << std::endl;
		return;
	}

	// to 배열에서 위에서부터 순차적으로 원반이 있는 칸을 찾는다. 원반이 있다면 바로 위 칸의 인덱스를 저장한다.
	// 옮기려는 기둥 맨 윗칸에 원반이 있을 수는 없으므로, 1번 인덱스부터 탐색한다.
	for (int i = 1; i < height; i++)
	{
		if (*(to + i) > 0)
		{
			// [예외 처리] 찾은 원반이 옮기려는 원반보다 작다면, 에러 메시지 출력 후 return
			if (*(to + i) < target)
			{
				std::cout << "옮기려는 기둥에 더 작은 원반이 있습니다." << std::endl;
				return;
			}

			toIndex = i - 1;
			break;
		}
	}

	// 기존에 원반이 있던 위치는 비우고, 원반이 들어갈 자리에 저장해둔 원반 번호를 대입한다.
	*(from + fromIndex) = 0;
	*(to + toIndex) = target;

	std::cout << ++moveCount << " : " << (char)('A' + *(from + height)) << "에서 " << (char)('A' + *(to + height)) << "로 원반 " << target << "을 옮깁니다." << std::endl;
}

/// <summary>
/// 게임 완료 여부를 체크한다. 목표 기둥의 맨 위칸에 원반이 있다면 true를 반환한다.
/// </summary>
/// <param name="lastColumn">원반을 가득 채우면 게임 종료로 판정할 마지막 기둥의 배열 포인터를 받는다.</param>
bool IsCompleted(int* lastColumn)
{
	if (*(lastColumn) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
