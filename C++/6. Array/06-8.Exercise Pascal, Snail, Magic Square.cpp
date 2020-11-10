#include <iostream>

int main()
{
    // #1. 파스칼의 삼각형 출력하기 (높이 : 5)
    std::cout << "#1. 파스칼의 삼각형" << std::endl;

    //삼각형의 높이에 따라, 해당 숫자만큼의 2차원 배열을 생성한다.
    const int HEIGHT{ 5 };
    int triangle[HEIGHT][HEIGHT]{};

    // 파스칼의 삼각형에 들어갈 숫자 입력
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x <= y; x++)
        {
            if (x == 0 || x == y)
            {
                triangle[y][x] = 1;
            }
            else
            {
                triangle[y][x] = triangle[y - 1][x] + triangle[y - 1][x - 1];
            }
        }
    }

    // 출력 형식에 맞게 숫자 출력
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int i = 0; i < HEIGHT - y; i++)
        {
            std::cout << " ";
        }
        
        for (int x = 0; x <= y; x++)
        {
            std::cout << triangle[y][x];
            if (x < y)
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    //---------------------------------------------------------------------






    // #2. 달팽이 만들기 (5 x 5 사이즈)
    std::cout << std::endl << "#2. 달팽이 만들기" << std::endl;

    // 크기에 따라 2차원 배열을 만들고, 달팽이가 회전할 방향(오른쪽을 시작점으로 우, 하, 좌, 상 순서)에 따라 1칸 나아갈 때 변경되는 x,y 좌표값을 입력해 둔다.
    const int SIZE{ 5 };

    int snail[SIZE][SIZE]{};

    int deltaY[4]{ 0, 1, 0, -1 };
    int deltaX[4]{ 1, 0, -1, 0 };
    int dir = 0;

    // 좌표 0,0을 시작점으로, 오른쪽 방향으로 전진하며 순차적으로 숫자를 기입한다.
    // 다음 좌표가 인덱스를 벗어나 있다면 오른쪽으로 회전한다. 회전 시 나아갈 방향은 앞에서 선언한 deltaX,Y에서 방향값 dir를 인덱스로 받아올 수 있다.
    int count{ SIZE * SIZE };
    int nowX{ 0 };
    int nowY{ 0 };
    for (int i = 1; i <= count; i++)
    {
        snail[nowY][nowX] = i;

        for (int j = 0; j < 4; j++)
        {
            int nextY{ nowY + deltaY[dir] };
            int nextX{ nowX + deltaX[dir] };

            if (nextY < 0 || nextY >= SIZE || nextX < 0 || nextX >= SIZE || snail[nextY][nextX] != 0)
            {
                dir = (dir + 1) % 4;
                continue;
            }
            else
            {
                nowY = nextY;
                nowX = nextX;
                break;
            }
        }
    }
    
    // 배열 출력
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            std::cout << snail[y][x] << "\t";
        }
        std::cout << std::endl;
    }
    //---------------------------------------------------------------------






    // #3. 홀수 마방진 만들기 (5 x 5 사이즈)
    std::cout << std::endl << "#3. 홀수 마방진 만들기" << std::endl;

    // 크기에 따라 2차원 배열을 만들고, 인덱스가 들어갈 배열 생성한다.
    int square[SIZE][SIZE]{};
    int index[SIZE]{};
    for (int i = 0; i < SIZE; i++)
    {
        index[i] = i;
    }

    // 시작 위치는 2차원 배열의 첫 번째 행의 가운데 열로 지정한다.
    nowX = SIZE / 2;
    nowY = 0;

    // 1부터 순서대로 숫자를 넣고, 규칙에 따라 다음 위치를 지정한다. 지정한 위치에 이미 숫자가 기입되어 있으면 1칸 밑을 다음 칸으로 지정한다.
    for (int i = 1; i <= count; i++)
    {
        square[nowY][nowX] = i;

        int nextY{ (nowY - 1) < 0 ? SIZE - 1 : nowY - 1 };
        int nextX{ (nowX + 1) % SIZE };

        if (square[nextY][nextX] != 0)
        {
            nextY = (nowY + 1) % SIZE;
            nextX = nowX;
        }
        nowY = nextY;
        nowX = nextX;
    }

    // 배열 출력
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            std::cout << square[y][x] << "\t";
        }
        std::cout << std::endl;
    }
}
