#include <iostream>
#include <memory>

// _aligned_malloc / _aligned_free �Լ��� ����
// ���� ���� �޸𸮸� ���������� �Ҵ��ϰ� ���� �� ����Ѵ�.
// Microsoft SIMD�� ���� 16����Ʈ ������ �����͸� �����ϴ� ���� ����� �̿��ؾ� �� ��� ����Ѵ�.
// * C4316 ������ �ذ��� �� �ִ�.


template<size_t T>
class MyAlign
{
public:
	// Ŭ������ ����/���� �����ڿ� �޸� ���� ���� �� ���� ������ �����ε��Ѵ�.
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};

// �޸� ������ �ʿ��� Ŭ������, ������ �޸� ������ �־ �� Ŭ������ ��ӹ޴´�.
class MyClass : public MyAlign<16>
{
public:
	MyClass() {}
	~MyClass() {}
};

// ���� �ش� Ŭ���� ���� �� �޸𸮰� ���ĵȴ�. ����Ʈ �����Ϳ��� �����ȴ�.
int main()
{
	MyClass pClass;
	std::unique_ptr<MyClass> spClass{ std::make_unique<MyClass>() };	// make_unique�� ���������� new�� �����
}
