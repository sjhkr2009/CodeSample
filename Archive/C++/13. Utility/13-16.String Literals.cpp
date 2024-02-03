#include <iostream>

// 스트링 리터럴

using namespace std::string_literals;

int main()
{
	// using namespace std::string_literals을 선언하고, 문자열 뒤에 s를 붙이면 std::string으로 취급한다.

	auto string = "ABCD"s;
	// 스트링 리터럴을 사용하지 않으면, auto는 const char* 타입으로 취급된다.
}