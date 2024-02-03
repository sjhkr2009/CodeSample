#include <iostream>

// 함수 포인터

int NormalFunc(int x) { return x; }
int SquareFunc(int x) { return x * x; }

// int (*f)(int x) : int를 매개변수로 int를 반환하는 f 함수의 포인터. 매개변수명 x는 생략가능.
// int *f(int x) : 단순히 f(int) 함수를 뜻하며 함수 포인터가 아님. 
int Sigma(int (*f)(int), int m, int n)
{
	int sum{};

	for (int i = m; i < n; i++)
	{
		sum += f(i);
	}

	return sum;
}

// 이를 좀더 간단하게 적고 싶으면 아래의 둘 중 하나를 쓰면 된다. using쪽이 직관성이 높아 권장된다.

// FunctionPtr로 int (*f)(int)를 대체할 수 있다.
typedef int (*FunctionPtr)(int);
using FunctionPtr = int(*)(int);

int Sigma2(FunctionPtr f, int m, int n)
{
	int sum{};

	for (int i = m; i < n; i++)
	{
		sum += f(i);
	}
	
	return sum;
}

int main()
{
	std::cout << Sigma(NormalFunc, 1, 10) << std::endl;
	std::cout << Sigma2(SquareFunc, 1, 10) << std::endl;
}
