#include <iostream>

// explicit를 붙이면 해당 타입은 암시적으로 형변환할 수 없다.

class MyClass
{
public:
	int mValue;
	MyClass(int v) : mValue{ v } {}
};

class MyClass2
{
public:
	int mValue;
	explicit MyClass2(int v) : mValue{ v } {}
};

// 연산자에도 적용된다.

class MyClass3
{
public:
	explicit operator int()
	{
		return 100;
	}
};

int main()
{
	MyClass c1 = MyClass(1);
	MyClass c2 = 2;

	std::cout << c1.mValue << '\t' << c2.mValue << std::endl;

	MyClass2 c3 = MyClass2(3);
	//MyClass2 c4 = 4; //explicit을 사용하면 이런 방식의 클래스 생성은 불가능해진다.

	//사실 그냥 유니폼 초기화식 쓰면 되므로, 다른 사람 코드 볼 때를 대비해서 알고만 있자.

	int value;

	//value = a; // 암시적 형변환 시 에러
	value = static_cast<int>(a); // 명시적 형변환이 강제된다.
}
