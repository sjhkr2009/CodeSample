#include <iostream>
#include <memory>

// _aligned_malloc / _aligned_free 함수로 구현
// 실제 물리 메모리를 순차적으로 할당하고 싶을 때 사용한다.
// Microsoft SIMD와 같이 16바이트 단위로 데이터를 전달하는 등의 방식을 이용해야 할 경우 사용한다.
// * C4316 에러를 해결할 수 있다.


template<size_t T>
class MyAlign
{
public:
	// 클래스의 생성/해제 연산자에 메모리 순차 정렬 및 해제 로직을 오버로딩한다.
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

// 메모리 정렬이 필요한 클래스에, 정렬할 메모리 단위를 넣어서 이 클래스를 상속받는다.
class MyClass : public MyAlign<16>
{
public:
	MyClass() {}
	~MyClass() {}
};

// 이후 해당 클래스 생성 시 메모리가 정렬된다. 스마트 포인터에도 대응된다.
int main()
{
	MyClass pClass;
	std::unique_ptr<MyClass> spClass{ std::make_unique<MyClass>() };	// make_unique도 내부적으로 new를 사용함
}
