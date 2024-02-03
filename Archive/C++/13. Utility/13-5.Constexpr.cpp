#include <iostream>

// constexpr : 뒤에 나오는 표현식(expression)을 상수화한다.
//	ㄴ 함수나 연산자에 붙을 경우 반환값이 상수가 된다.

// case문처럼 상수가 필요한 구문이 있다.
// Enum을 연산하면 반환값이 변수가 되는데, constexpr int로 오버라이딩하면 컴파일 타임에 상수가 필요한 부분에서 이 연산자를 사용, 반환값이 상수가 된다.
// 즉 constexpr int 반환값으로 연산한 값은 case문의 조건이 될 수 있다.

enum Flags
{
	slow,
	airborne,
	poison
};

constexpr int operator | (Flags f1, Flags f2)
{
	return Flags(int(f1) | int(f2));
}

int main()
{
	Flags status{};

	switch (status)
	{
		case slow | poison:
			// enum 간의 연산이지만 반환값을 상수로 판단하여 case문의 조건이 될 수 있다.
			break;
		case airborne:
			break;
		default:
			break;
	}
}