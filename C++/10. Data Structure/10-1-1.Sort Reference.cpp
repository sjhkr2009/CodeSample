#include <iostream>

void PrintArray(int input[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << input[i] << " ";
	}
}

void Swap(int& v1, int& v2)
{
	int temp = v1;
	v1 = v2;
	v2 = temp;
}

void SequentialSort(int input[], int size)
{
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = i + 1; j < size; ++j)
		{
			if (input[i] > input[j])
			{
				Swap(input[i], input[j]);
			}
		}
	}
}

void SelectionSort(int input[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		int minIndex{ i };

		for (int j = i; j < size; ++j)
		{
			if (input[minIndex] > input[j])
			{
				minIndex = j;
			}
		}

		if (i != minIndex)
		{
			Swap(input[i], input[minIndex]);
		}
	}
}

void BubbleSort(int input[], int size)
{
	for (int phase = 0; phase < size - 1; ++phase)
	{
		for (int i = 0; i < size - phase - 1; ++i)
		{
			if (input[i] > input[i + 1])
			{
				Swap(input[i], input[i + 1]);
			}
		}
	}
}

void InsertionSort(int input[], int size)
{
	for (int i = 1; i < size; ++i)
	{
		int j = i;
		int target = input[i];

		while (--j >= 0 && target < input[j])
		{
			input[j + 1] = input[j];
			input[j] = target;
		}
	}
}

void Merge(int input[], int start, int half, int end, int temp[])
{
	int i{ start };
	int j{ half + 1 };
	int tempIndex{ 0 };

	while (i <= half && j <= end)
	{
		if (input[i] < input[j])
		{
			temp[tempIndex++] = input[i++];
		}
		else
		{
			temp[tempIndex++] = input[j++];
		}
	}

	// 왼쪽 나머지들 병합
	while (i <= half)
	{
		temp[tempIndex++] = input[i++];
	}
	// 오른쪽 나머지들 병합
	while (j <= end)
	{
		temp[tempIndex++] = input[j++];
	}

	// input 에 복사
	tempIndex = 0;
	for (int i = start; i <= end; ++i)
	{
		input[i] = temp[tempIndex++];
	}
}

void MergeSort(int input[], int start, int end, int temp[])
{
	if (start >= end)
		return;

	int half{ (start + end) / 2 };
	MergeSort(input, start, half, temp);
	MergeSort(input, half + 1, end, temp);

	Merge(input, start, half, end, temp);
}

void QuickSort(int input[], int left, int right)
{
	int i{ left };
	int j{ right };
	int pivot{ input[(left + right) / 2] };

	do
	{
		// 피벗을 기준으로 왼쪽에 작은값 - left 값이 피벗 보다 작으면 증가
		while (input[i] < pivot)
		{
			i++;
		}
		// 피벗을 기준으로 오른쪽에 큰값 - right 값이 피벗 보다 크면 감소
		while (input[j] > pivot)
		{
			j--;
		}

		if (i <= j)
		{
			Swap(input[i], input[j]);
			i++;
			j--;
		}
	} while (i <= j);

	// 왼쪽 블럭
	if (left < j)
	{
		QuickSort(input, left, j);
	}

	// 오른쪽 블럭
	if (i < right)
	{
		QuickSort(input, i, right);
	}
}


int main()
{
	const int size = 5;
	int array[size]{ 8, 7, 2, 3, 1 };

	// sort
	//SequentialSort(array, size);
	//SelectionSort(array, size);
	//BubbleSort(array, size);
	//InsertionSort(array, size);

	//int temp[size]{};
	//MergeSort(array, 0, 4, temp);

	//QuickSort(array, 0, 4);

	PrintArray(array, size);
}


