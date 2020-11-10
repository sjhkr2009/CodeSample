#include "Dog.h"
#include <iostream>

Dog::Dog() :
	mBreed{ BREED::NONE }
{
	std::cout << "Dog Created!" << std::endl;
}

// �θ� Ŭ������ ���� �ڽ� Ŭ�������� �ʱ�ȭ�ϱ� - �ڽ� Ŭ�������� �θ� Ŭ���� ������ ���� �ʱ�ȭ�ϴ� ���� ������� ������, ������ ����� ���� �ذ��Ѵ�

// �θ� Ŭ������ �ʱ�ȭ�� ���� �Ű������� �޴� �����ڸ� �����, �ش� �����ڸ� �޾ƿ´�.
// �θ� Ŭ���� �������� �Ű������� �ڽ� Ŭ���� �������� �Ű������� �����Ѵ�.
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
