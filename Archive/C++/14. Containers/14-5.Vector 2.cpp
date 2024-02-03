#include <iostream>
#include <vector>
#include <algorithm>

int CompareFunc(const void* p1, const void* p2)
{
	int left = *static_cast<const int*>(p1);
	int right = *static_cast<const int*>(p2);
	/*if (left > right)
	{
		return -1;
	}
	if (left < right)
	{
		return 1;
	}
	return 0;*/

	//위 내용을 간단히 표현하면
	return right - left;
}


int main()
{
	std::vector<int> v{};

	// 조건이 맞을 때까지 반복 입력 받기
	//while (true)
	//{
	//	int value;

	//	if (std::cin >> value) // 사용자 입력을 가져왔다면 벡터에 추가하고
	//	{
	//		v.push_back(value);
	//	}
	//	else // 받을 수 없는 입력(문자 등)이면 반복문을 빠져나온다.
	//	{
	//		break;
	//	}
	//}

	// 위 동작은 아래와 같이 줄일 수 있다.
	for (int value; std::cin >> value ;) // value 선언 -> 조건 체크: (입력이 int에 들어왔는가?) -> 반복 시 시행할 동작 없음
	{
		v.push_back(value);
	}

	int sum{};
	for (auto& element : v)
	{
		sum += element;
	}

	float average{ static_cast<float>(sum) / v.size() };

	std::cout << "합: " << sum << std::endl;
	std::cout << "평균: " << average << std::endl;

	//-----------------------------------------------------------------


	std::vector<int> v1{ 3 };	// 3이라는 원소를 넣어서 생성 (size : 1) -> { 3 }
	std::vector<int> v2(3);		// 3개의 원소가 들어갈 공간을 생성 (size : 3) -> {0, 0, 0}
	// ()를 사용하면 vector의 초기화식을 통해 해당 개수만큼의 원소가 생성됨

	for (auto& e : v1)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;

	for (auto& e : v2)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;

	//-----------------------------------------------------------------

	// std::qsort : 배열 정렬하기
	// 매개변수에 (배열의 데이터(.data), 크기(.size), 시작 원소의 크기(sizeof), 비교함수) 를 입력한다.
	// 비교함수는 어느 쪽이 우선시되는지를 의미한다.

	std::vector<int> v3{ 3, 5, 6, 1, 9, 2 };

	std::qsort(v3.data(), v3.size(), sizeof(int), CompareFunc); //오름차순 정렬
	for (auto& e : v3)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;

	// 람다식으로 비교함수를 구현한 내림차순 정렬
	std::qsort(v3.data(), v3.size(), sizeof(int),
		[](const void* a, const void* b) { return *static_cast<const int*>(a) - *static_cast<const int*>(b); }
		);

	for (auto& e : v3)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;


	//-----------------------------------------------------------------

	// std::for_each : 배열 순회하기
	// 매개변수에 (탐색 시작 부분, 탐색 끝 부분, 각 요소에 행할 작업을 서술한 함수) 를 입력한다.
	// #include <algorithm> 필요

	std::for_each(v3.begin(), v.end(),
		[](const int& val) { std::cout << val << ' ';}
		);
}
