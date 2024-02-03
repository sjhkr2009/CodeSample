v#include<iostream>

int main()
{
	//for문 - 1부터 10까지 출력하기
	
	for (int i = 0; i < 10; ++i) //기본 형태는 (초기화;조건식;증감식 또는 반복마다 시행할 동작)
	{
		std::cout << i + 1 << '\t';
	}
	std::cout << std::endl;

	int i{ 1 };
	for (;;) //생략도 가능하다. 단, 초기화/조건식/증감을 모두 고려하지 않으면 에러나 무한루프에 빠질 수 있으니, 가급적 기본 형태는 사용하자.
	{
		std::cout << i++ << '\t';
		if (i > 10)
		{
			std::cout << std::endl;
			break;
		}
	}

	for (int i = 0, j = 1; i < 10; i++, ++j) //여러 개의 변수를 선언할 수도 있다.
	{

	}


	//for문 - 1부터 10까지 홀수 출력하기

	for (int i = 1; i <= 10; ++i)
	{
		if ((i % 2) != 0)
		{
			std::cout << i << '\t';
		}
	}
	std::cout << std::endl;

	for (int i = 0; i < 10; i += 2) //결과에서 패턴(2씩 증가)을 찾아낸다면, 조건식 계산 X, for문 다섯 번만 실행되도록 바꿀 수 있다. 최적화의 기본적인 형태.
	{
		std::cout << (i + 1) << '\t';
	}
	std::cout << std::endl;


	//for문 - 0부터 99까지 제곱

	for (int i = 0; i < 100; i++)
	{
		std::cout << i << "의 제곱 = " << i * i << std::endl;
	}
	std::cout << std::endl;
	//시작과 끝이 정해진 동작은 while보다 for이 편리하다.
}