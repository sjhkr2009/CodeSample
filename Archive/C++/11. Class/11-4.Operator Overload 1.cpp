#include <iostream>

class RealNumber
{
public:
	int i;
	double f;

	// 클래스에 연산자를 추가할 수 있다.
	RealNumber operator + (RealNumber& op) //operator 연산자 (연산 대상) { 연산 내용 및 return 값 } 형태를 가진다.
	{
		RealNumber c;
		c.f = f + op.f;
		c.i = i + op.i;
		return c;
	}
};

class Parent
{
public:
	void function() { std::cout << "Parent" << std::endl; }
};

class Child : public Parent
{
public:
	void function() { std::cout << "Child" << std::endl; }
};

int main()
{
	// 연산자 오버로드

	RealNumber c1, c2, c3;

	c1.i = 2;
	c1.f = 0.2f;

	c2.i = 5;
	c2.f = 0.1f;

	c3 = c1 + c2;

	std::cout << c3.f + c3.i << std::endl;

	// 참고) 함수 오버라이딩
	
	Child c;

	c.function(); //자식 오브젝트에서 (오버라이딩 없이) 동일한 이름의 함수를 생성하면, 부모 클래스의 함수를 덮어쓴다.
	static_cast<Parent>(c).function();
}