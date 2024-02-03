#include <iostream>

class Animal
{
public:
	int age;

public:
	void Walk() { std::cout << "ANIMAL WAKLING" << std::endl; }
};

class Dog : public Animal
{
public:
	Dog()
	{
		std::cout << "개 생성자 호출" << std::endl;
	}
	~Dog()
	{
		std::cout << "개 파괴자 호출" << std::endl;
	}
public:
	char* breed{};

public:
	void Bark() {};
};

int main()
{
	Dog john;

	std::cout << "john 변수 선언" << std::endl;

	john = Dog();

	std::cout << "john 변수에 Dog() 할당" << std::endl;

	john.age = 1;
	john.Walk();

	//------------------------------------------------------------------

	Dog* merry;
	
	std::cout << "merry 변수 선언" << std::endl;

	merry = new Dog(); // 클래스 포인터로 생성 시, 다른 포인터 변수처럼 new 키워드를 붙인다.

	Animal* pAnimal{ static_cast<Animal*>(merry) };
	Dog* pDog{ static_cast<Dog*>(pAnimal) }; // 부모-자식 클래스 간의 캐스팅이 가능하다

	pAnimal->age = 1; // Animal의 변수,함수에만 접근 가능
	pDog->breed = new char[8]{ "john" }; // Dog의 변수,함수에도 접근 가능

	delete merry;
	std::cout << "merry 변수 해제" << std::endl;

}