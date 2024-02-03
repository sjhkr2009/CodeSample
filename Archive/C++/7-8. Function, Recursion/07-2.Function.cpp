#include <iostream>

//입력받은 단 수의 구구단을 출력하는 함수
void GuGuDan(int);

//A를 B로 나눈 몫을 구하는 함수
int Quotient(int, int);

//A를 B로 나눈 나머지를 구하는 함수
int Remainder(int, int);

int main()
{
    int index{};

    std::cout << "몇 단을 출력할까요? : ";
    std::cin >> index;

    GuGuDan(index);

    int a{}, b{};
    std::cout << "나눌 숫자 a,b를 입력하세요. (입력 예시: 17 4)" << std::endl << ">> ";
    std::cin >> a >> b;

    std::cout << a << "를 " << b << "로 나눈 몫은 " << Quotient(a, b) <<
        ", 나머지는 " << Remainder(a, b) << " 입니다." << std::endl;
}

void GuGuDan(int index)
{
    for (int i = 1; i <= 9; i++)
    {
        std::cout << index << " x " << i << " = " << (index * i) << std::endl;
    }
}

int Quotient(int big, int small)
{
    if (small == 0)
    {
        std::cout << "0으로 나눌 수 없습니다." << std::endl;
        return -1;
    }
    return big / small;
}

int Remainder(int big, int small)
{
    if (small == 0)
    {
        std::cout << "0으로 나눌 수 없습니다." << std::endl;
        return -1;
    }
    int quot = Quotient(big, small);
    int result = big - (quot * small);
    return result;
}

