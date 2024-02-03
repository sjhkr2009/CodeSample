#include <iostream>

class ClassA
{
public:
	ClassA(std::string name)
	{
		this->name = name; // 이름이 같은 멤버 변수는 this를 통해 매개변수와 구분해준다.
		std::cout << name << " 가 생성되었습니다." << std::endl;
	}
	ClassA(int index = 0, int data = 0) :
		mIndex{ index },
		mData(data)
		// 매개변수를 멤버 변수에 대입하는 동작은 생성자 선언 뒤에 위와 같이 표기할 수도 있다. 이 때 복사 형태의 초기화는 불가능하다.
	{
		std::cout << mIndex << "번 클래스가 생성되었습니다." << std::endl;
	}

public:
	std::string name;
	int mIndex;
	int mData;
};

int main()
{
	ClassA* myClass1 = new ClassA("My Class");
	ClassA* myClass2 = new ClassA(1);
	
	int v1 = 2;			// 1. copy initialization : 값을 복사해서 변수에 대입하는 방식
	int v2(2);			// 2. direct initialization : int도 내부적으로 object타입의 클래스이므로, 클래스 초기화식으로 직접 값을 초기화할 수 있다.
	int v3{ 2 };		// 3. uniform initialization : 어떤 타입이든 {} 를 통해 공통적으로 초기화할 수 있도록 만든 초기화식.
	int v4 = { 2 };		// 4. uniform copy initialization : 유니폼 초기화식을 통해 생성한 값을 복사하여 대입하는 방식.

	// copy 타입의 복사인 1,4번 방식이 2,3번에 비해 상대적으로 느리다.

	ClassA a1 = ClassA(); // 만약 클래스의 크기가 100MB라면, 100MB 분량의 복사가 발생한다. 따라서 클래스는 2,3번 방식으로 생성하는 것이 권장된다.
	ClassA a2();
	ClassA a3{};
	ClassA a4 = {};
}