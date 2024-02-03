#include<iostream>

int main()
{
	int scoresA[10]{};

	for (int i = 0; i < 10; ++i)
	{
		scoresA[i] = i + 1;
		std::cout << i << " : " << scoresA[i] << std::endl;
	}

	int scoresB[10]{ 1,2,3 }; //앞의 3개 값만 1,2,3이 할당되고, 나머지는 기본값인 0이 할당된다.

	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " : " << scoresB[i] << std::endl;
	}

	int scoresC[]{ 1,2,3,4,5,6,7,8,9,10 }; //모든 값을 초기화해줄 경우 배열 개수를 명시하지 않을 수도 있다. 다만 이 경우에도 명시적으로 적어주는 게 좋다.

	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " : " << scoresC[i] << std::endl;
	}
}