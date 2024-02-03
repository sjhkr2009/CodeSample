#include <iostream>

// 메모리 정렬

// alignas(x) 를 구조체 앞에 붙이면, x바이트 만큼 메모리 공간을 할당한다. 남는 공간은 비어있게 된다.
// 필요한 공간보다 적게 할당하면 에러가 난다.

struct alignas(16) MyStruct //MyStruct에 16바이트 할당
{
	int myInt{};
	bool myBool{};
};

int main()
{
	
}