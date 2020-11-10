#include <iostream>

// 함수명() noexcept : 예외가 발생하지 않는다는 의미

// inline: 호출 스택에 의한 부하를 줄여준다.
// inline 함수 : 호출 스택 없이 해당 코드로 바로 연결되는 함수
//		ㄴ 해당 함수 호출 시 해당 함수 내용을 그대로 가져온다. (#define 과 유사하나 define은 컴파일 단계에서 항상 바꾸고, inline은 최적화 과정에서 최적화가 가능한 경우에만 바꾼다.)
//		ㄴ 호출 스택을 쓰지 않으므로 더 빠르지만 코드를 모두 가져오므로 파일 크기는 커진다.
//		ㄴ 선언과 정의를 분리해서는 안 되며, 헤더에 정의된 전역 함수일 경우 재정의로 발생하는 에러를 방지할 수 있다.
// inline namespace : 네임스페이스 내에 네임스페이드를 선언할 때 사용
//		ㄴ 인라인 네임스페이스 내에 있는 함수나 변수는, 모호하지 않은 경우 범위 결정 연산자 :: 를 생략할 수 있다.

// 사용자 정의 리터럴: 연산자 오버로딩을 통해 ""리터럴 형태를 사용하면 사용자 정의 리터럴을 만들 수 있다.
//					ㄴ 매개변수는 long double 타입만 가능하다.
int operator ""km(long double value)
{
	return static_cast<int>(value);
}

namespace NAME1
{
	int a{ 1 };
	namespace NAME2
	{
		int a{ 2 };
	}

	inline namespace NAME3
	{
		int a{ 3 };
	}
}

void Func1() noexcept { };

int main()
{
	int a{ 1.5km }; // 사용자 정의 리터럴
	std::cout << a;
}