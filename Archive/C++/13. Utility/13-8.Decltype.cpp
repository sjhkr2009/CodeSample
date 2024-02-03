#include <iostream>

// decltype(x) : x의 타입을 가져오는 기능

// 반환형 접미사 ->
// 일반화된 요소를 연산하여 반환형이 매번 다를 경우, auto 키워드를 쓰고 -> decltype 로 반환형을 지정할 수 있다.
// 매개변수를 받은 후 decltype을 통해 타입을 결정하도록 미루는 방식.

template<class T, class U>
auto Add(T x, U y) -> decltype(x + y)
{
	return x + y;
}


int main()
{
	int x{ 1 };
	decltype(x + 3) y{ 1 }; // x + 3의 타입인 int를 가져온다. 즉 int y{1} 과 같다.
}