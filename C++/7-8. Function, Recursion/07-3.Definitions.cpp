#include <iostream>

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