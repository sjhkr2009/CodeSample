#include <iostream>

// 퀵 정렬 구현하기

void QuickSort(int input[], int size);
void QuickSort(int input[], int left, int right);

int main()
{
	const int SIZE{ 9 };
	int array[SIZE]{ 6,9,4,5,8,3,2,1,7 };
	
	QuickSort(array, SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		std::cout << array[i] << ' ';
	}
}

void QuickSort(int input[], int size)
{
	QuickSort(input, 0, size - 1);
}

void QuickSort(int input[], int left, int right)
{
	int pivot{ input[(left + right) / 2] };
	int indexL{ left };
	int indexR{ right };

	do
	{
		while (input[indexL] < pivot)
		{
			indexL++;
		}
		while (input[indexR] > pivot)
		{
			indexR--;
		}
		if (indexL <= indexR)
		{
			int temp{ input[indexL] };
			input[indexL] = input[indexR];
			input[indexR] = temp;
			indexL++;
			indexR--;
		}
	} while (indexL <= indexR);

	if (left < indexR)
	{
		QuickSort(input, left, indexR);
	}
	if (right > indexL)
	{
		QuickSort(input, indexL, right);
	}
}
