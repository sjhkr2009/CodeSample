#include <iostream>

// 멤버 함수 포인터

class MyFunctions
{
public:
	static int NormalFunc(int x) { return x; }
	int SquareFunc(int x) { return x * x; }

};

// 정적 멤버 함수의 포인터는 호출 시를 제외하면 일반 함수와 동일하다.
using FunctionPtr = int(*)(int);
// 비정적 멤버 함수는 범위 해결 연산자(::)를 사용한다.
using pFuncMyclass = int(MyFunctions::*)(int);

int Sigma(FunctionPtr f, int m, int n)
{
	int sum{};

	for (int i = m; i < n; i++)
	{
		sum += f(i);
	}

	return sum;
}

// 비정적 멤버 함수 사용 시 함수를 호출할 인스턴스를 참조형으로 받는다.
int Sigma2(MyFunctions& instance, pFuncMyclass f, int m, int n)
{
	int sum{};

	for (int i = m; i < n; i++)
	{
		// 사용 시 인스턴스를 통해 함수를 호출한다.
		sum += (instance.*f)(i);
	}

	return sum;
}

int main()
{
	MyFunctions mf;

	// 멤버 함수의 포인터는 호출 시 &를 붙여 명시적으로 주소로 변환한다. (일반 함수는 암시적으로 변환됨)
	std::cout << Sigma(&MyFunctions::NormalFunc, 1, 10) << std::endl;
	std::cout << Sigma2(mf, &MyFunctions::SquareFunc, 1, 10) << std::endl;
}
