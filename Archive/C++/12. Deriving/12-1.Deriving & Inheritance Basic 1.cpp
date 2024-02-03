#include <iostream>

class Base
{
public:
	int p1;

protected:
	int p2;

private:
	int p3;
};

class Derived1 : private Base
{
	
};

class Derived2 : public Derived1
{
	void Function()
	{
		//this->p1
		//p1은 부모 클래스에서 private으로 상속되었으므로, 더 이상 사용할 수 없다.
	}
};

int main()
{
	
}