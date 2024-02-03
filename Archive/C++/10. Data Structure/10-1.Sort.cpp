#include <iostream>

#pragma region 선언부 및 설명

void PrintArray(int input[], int size)
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << input[i] << " ";
	}
}

void Swap(int& v1, int& v2)
{
	int temp{ v1 };
	v1 = v2;
	v2 = temp;
}

// 1. 순차 정렬(Sequential Sort): 각 인덱스마다 나를 제외한 다음 요소들을 순차적으로 비교해서, 가장 작은 값을 순차적으로 채운다.
// 시간 복잡도 : O(n^2)	-> 1 + 2 + 3 + ... + (n - 1) -> 시그마 합계 최고차항은 1/2(n^2)
// 공간 복잡도 : O(n)	-> 매개변수로 받은 배열만 사용하므로 n
void SequentialSort(int input[], int size);

// 2. 선택 정렬(Selection Sort): 사람의 수행방식과 비슷하다. 모든 배열을 스캔하여 가장 작은 요소를 찾아 앞으로 가져오기를 반복한다.
// 시간 복잡도 : O(n^2)	-> 위와 같음
// 공간 복잡도 : O(n)
void SelectionSort(int input[], int size);

// 3. 거품 정렬(Bubble Sort): 인접한 두 개의 원소를 검사하여 큰 값을 뒤로 보낸다.
// 시간 복잡도 : O(n^2)	-> 위와 같음
// 공간 복잡도 : O(n)
void BubbleSort(int input[], int size);
void BubbleSort_Reference(int input[], int size);

// 4. 삽입 정렬(Insert Sort): 각 인덱스마다 내 앞의 인덱스 원소와 자신을 비교해서, 해당 원소가 더 작으면 두 원소의 자리를 바꾼다.
// 시간 복잡도 : O(n^2)	-> (최악의 경우를 상정하면) 위와 같음
// 공간 복잡도 : O(n)
void InsertSort(int input[], int size);

// 5. 병합 정렬(Merge Sort): Divide & Conquer, 문제를 세분화하되 더 이상 나눌 수 없을 때 동작을 수행한다.
// 시간 복잡도: O(n log(n)) -> 절반씩 분할하므로 log2에 해당한다. 즉 나눌 때 n log2(n) + 합칠 때 2n log2(n) = 3n log2(n) -> 계수와 밑은 버리고 n과 log(n) 만 남음.
// 공간 복잡도: 0(n) -> Sort의 대상인 array와 임시 배열 temp 둘을 사용하므로 2n
void MergeSort(int input[], int start, int end, int temp[]);
void MergeSort_Merge(int input[], int start, int half, int end, int temp[]);

// 6. 빠른 정렬(Quick Sort): 특정 값을 pivot으로 배열을 나누어 정렬하고, 나눈 각 블럭에 대한 정렬을 반복한다. 병합 과정 없이 나누면서 동시에 정렬한다.
// 시간 복잡도: O(n log(n)) -> 분할은 pivot 중심으로 나누므로 n log n, 정렬은 QuickSort를 몇 번 반복할지 모르니 상수로 취급한다.
//				단, 최악의 경우 n^2까지 증가할 수 있는데, pivot을 잘 선택하는 것이 중요하다. 평균적으로는 n log n의 시간복잡도를 가진다.
// 공간 복잡도: O(n) -> 배열 1개만 사용 (배열이 아닌 임시 변수들은 상수이므로 생략)
void QuickSort(int input[], int left, int right);

#pragma endregion

int main()
{
	const int size{ 7 };

	int array[size]{ 5,8,2,4,1,9,3 };

	// sort 처리 부분
	//SequentialSort(array, size);
	//SelectionSort(array, size);
	//BubbleSort(array, size);
	//BubbleSort_Reference(array, size);
	//InsertSort(array, size);

	//int temp[size]{};
	//MergeSort(array, 0, size - 1, temp);

	QuickSort(array, 0, size - 1);

	PrintArray(array, size);
}

void SequentialSort(int input[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
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
	for (int i = 0; i < size - 1; i++)
	{
		int minIndex{ i };
		for (int j = i + 1; j < size; j++)
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
	for (int i = (size - 1); i > 0; --i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (input[j] > input[j + 1])
			{
				Swap(input[j], input[j + 1]);
			}
		}
	}
}

void BubbleSort_Reference(int input[], int size)
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

void InsertSort(int input[], int size)
{
	for (int i = 1; i < size; i++)
	{
		int j{ i };
		int target{ input[i] };

		while (--j >= 0 && target < input[j])
		{
			input[j + 1] = input[j];
			input[j] = target;
		}
	}
}

void MergeSort(int input[], int start, int end, int temp[])
{
	if (start >= end)
		return;

	int half{ (start + end) / 2 };
	MergeSort(input, start, half, temp);
	MergeSort(input, half + 1, end, temp);

	MergeSort_Merge(input, start, half, end, temp);
}

void MergeSort_Merge(int input[], int start, int half, int end, int temp[])
{
	int i{ start };
	int j{ half + 1 };
	int tempIndex{ 0 };

	// 두 부분배열을 임시배열 temp에 합치기
	while (i <= half && j <= end)
	{
		if (input[i] < input[j])
		{
			temp[tempIndex] = input[i];
			tempIndex++;
			i++;
		}
		else
		{
			temp[tempIndex] = input[j];
			tempIndex++;
			j++;
		}
	}

	//왼쪽 나머지들 병합
	while (i <= half)
	{
		temp[tempIndex] = input[i];
		tempIndex++;
		i++;
	}

	//오른쪽 나머지들 병합
	while (j <= end)
	{
		temp[tempIndex] = input[j];
		tempIndex++;
		j++;
	}

	//input에 복사
	tempIndex = 0;
	for (int i = start; i <= end; i++)
	{
		input[i] = temp[tempIndex++];
	}
}

void QuickSort(int input[], int left, int right)
{
	int i{ left };
	int j{ right };
	int pivot{ input[(i + j) / 2] };

	do
	{
		// 피벗 기준으로 왼쪽에서 작은 값 찾기 = left 값이 피벗보다 작으면 인덱스를 증가시키며 찾는다.
		while (input[i] < pivot)
		{
			i++;
		}

		// 피벗 기준으로 오른쪽에서 큰 값 찾기= right 값이 피벗보다 크면 인덱스를 감소시키며 찾는다.
		while (input[j] > pivot)
		{
			j--;
		}

		// 찾은 두 값이 교차되지 않는지 확인한 후, 바꿔준다. 바꾼 후에는 i와 j를 한 칸씩 전진시킨다.
		if (i <= j)
		{
			Swap(input[i], input[j]);
			i++;
			j--;
		}
	} while (i <= j);


	// 왼쪽,오른쪽 블럭에 동일한 작업 반복. 단, 배열의 범위가 초과되지 않도록 한다.
	// (블럭이 1개면 left == j, right == i 이므로 더 이상 QuickSort를 호출하지 않는다)
	if (left < j)
	{
		QuickSort(input, left, j);
	}
	if (right > i)
	{
		QuickSort(input, i, right);
	}

}
