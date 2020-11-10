#include <iostream>

// 병합 정렬 구현하기

void MergeSort(int input[], int temp[], int size);
void MergeSort_Divide(int input[], int start, int end, int temp[]);
void MergeSort_Merge(int input[], int start, int half, int end, int temp[]);

int main()
{
	const int SIZE{ 7 };
	int array[SIZE]{ 5,8,2,4,1,9,3 };
	int temp[SIZE]{};

	MergeSort(array, temp, SIZE);

	for (int i = 0; i < SIZE; i++)
	{
		std::cout << array[i] << ' ';
	}
}

void MergeSort(int input[], int temp[], int size)
{
	MergeSort_Divide(input, 0, size - 1, temp);
}

void MergeSort_Divide(int input[], int start, int end, int temp[])
{
	if (start >= end)
	{
		return;
	}

	int half = (start + end) / 2;
	MergeSort_Divide(input, start, half, temp);
	MergeSort_Divide(input, half + 1, end, temp);

	MergeSort_Merge(input, start, half, end, temp);
}

void MergeSort_Merge(int input[], int start, int half, int end, int temp[])
{
	int index1{ start };
	int index2{ half + 1 };
	int tempIndex{ 0 };

	while (index1 <= half && index2 <= end)
	{
		if (input[index1] < input[index2])
		{
			temp[tempIndex++] = input[index1++];
		}
		else
		{
			temp[tempIndex++] = input[index2++];
		}
	}

	while (index1 <= half)
	{
		temp[tempIndex++] = input[index1++];
	}

	while (index2 <= end)
	{
		temp[tempIndex++] = input[index2++];
	}
	
	for (int i = start, tempIndex = 0; i <= end; i++, tempIndex++)
	{
		input[i] = temp[tempIndex];
	}
}
