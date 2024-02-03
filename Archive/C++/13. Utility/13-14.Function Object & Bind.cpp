#include <iostream>
#include <functional>

// 함수 객체(Function Objecct, Functor) - include functional 필요
// 함수를 객체처럼 선언하여 사용하는 것

void MyFunction(const int arg)
{
	std::cout << arg << std::endl;
}

struct MyStruct
{
	void operator() (int a)
	{
		std::cout << "My Struct : " << a << std::endl;
	}
};

int main()
{
	std::function<void(const int)> functor1 = MyFunction;
	std::function<void(int)> functor2 = MyStruct();

	std::function<void(int)> functor3 = [](float a) { std::cout << "Call Lambda : " << a << std::endl; };
	std::function<void(float)> functor4 = [](int a) { std::cout << "Call Lambda : " << a << std::endl; };
	std::function<void(float)> functor5 = [](float a) { std::cout << "Call Lambda : " << a << std::endl; };

	functor1(1);
	functor2(2);
	functor3(3.3f);
	functor4(2.7f);
	functor5(1.6f);
}