#include <iostream>

int Sum1(int param[], int size);
int Sum2(int* param, int size);

int main()
{
	int score[7]{ 1,2,3,4,5,6,7 };
	int score2[3]{ 1,2,3 };

	std::cout << Sum1(score, 7) << std::endl;
	std::cout << Sum1(score2, 3) << std::endl;

	std::cout << Sum2(score, 7) << std::endl; // 배열은 포인터이므로, 포인터 매개변수 자리에도 배열을 넣을 수 있다.
	std::cout << Sum2(score2, 3) << std::endl;

	int num{ 1 };
	std::cout << Sum2(&num, 1) << std::endl; // 다만 매개변수가 포인터일 경우, 이런 식으로 의도치 않은 값을 넣을 수도 있다.
	// 따라서 매개변수가 배열일 경우 Sum1 처럼 정확히 배열임을 명시하는게 좋다.
}

// 배열을 매개변수로 받기
int Sum1(int param[], int size)
{
	int result{ 0 };

	for (int i = 0; i < size; i++)
	{
		result += param[i];
	}

	return result;
}

// 포인터를 매개변수로 받기
int Sum2(int* param, int size)
{
	int result{ 0 };

	for (int i = 0; i < size; i++)
	{
		result += param[i]; // 포인터에도 인덱스를 쓸 수 있다.
	}

	return result;
}