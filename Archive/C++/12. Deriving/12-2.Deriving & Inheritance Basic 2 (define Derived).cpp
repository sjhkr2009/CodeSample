#include "Dog.h"
#include <iostream>

Dog::Dog() :
	mBreed{ BREED::NONE }
{
	std::cout << "Dog Created!" << std::endl;
}

// 부모 클래스의 값을 자식 클래스에서 초기화하기 - 자식 클래스에서 부모 클래스 변수를 직접 초기화하는 것은 권장되지 않으며, 생성자 상속을 통해 해결한다

// 부모 클래스에 초기화할 값을 매개변수로 받는 생성자를 만들고, 해당 생성자를 받아온다.
// 부모 클래스 생성자의 매개변수에 자식 클래스 생성자의 매개변수를 대입한다.
Dog::Dog(int age, int weight, Dog::BREED breed) :
	Animal(age, weight),
	mBreed{ breed }
{
	std::cout << "Dog Created! (set Age, Weight, Breed)" << std::endl;
}

Dog::~Dog()
{
	std::cout << "Dog Destroyed!" << std::endl;
}

Dog::BREED Dog::GetBreed() const
{
	return mBreed;
}

void Dog::SetBreed(Dog::BREED breed)
{
	mBreed = breed;
}

void Dog::Rolling()
{
	std::cout << "Rolling..." << std::endl;
}
