#include <iostream>

// 함수 포인터

// 기획자가 스크립트 언어를 통해 게임 내 동작을 제어하는 경우가 있다.
// MoveForward(), Swing() 등의 함수가 있을 때, 특정 스크립트를 통해 함수를 실행시킬 수 있어야 한다.
// 이 때 함수를 가리키는 포인터를 사용하게 된다.



// m부터 n까지를 모두 더하는 함수 Sigma가 있을 때, m~n을 더하는 동작과 m^2 ~ n^2 를 더하는 동작을 하나의 함수에서 사용하기.

int Sigma(int f(int), int m, int n); //입력받은 m부터 n까지, f(x) 처리된 값을 모두 더하여 반환하는 함수
int f(int x); // x를 반환
int g(int x); // x의 제곱을 반환

int main()
{
	int sum{};
	sum = Sigma(f, 1, 10); // 1부터 10까지의 숫자 합
	std::cout << sum << std::endl;

	sum = Sigma(g, 1, 10); // 1부터 10까지의 숫자의 제곱 합
	std::cout << sum << std::endl;
}

// 함수는 무조건 포인터형으로 넘겨준다.
// 아래의 첫 번째 매개변수는 내부적으로 (*f)(int) 와 같이 동작한다.
int Sigma(int f(int), int m, int n)
{
	int sum{ 0 };
	for (int i = m; i <= n; i++)
	{
		sum += f(i);
	}
	return sum;
}

int f(int x)
{
	return x;
}

int g(int x)
{
	return x * x;
}
