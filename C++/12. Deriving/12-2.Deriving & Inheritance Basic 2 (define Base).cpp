#include "Animal.h"
#include <iostream>

Animal::Animal() :
	mAge{ 1 },
	mWeight{ 1 }
{
	std::cout << "Animal Created!" << std::endl;
}

Animal::Animal(int age, int weight) :
	mAge{ age },
	mWeight{ weight }
{
	std::cout << "Animal Created! (set Age, Weight)" << std::endl;
}

Animal::~Animal()
{
	std::cout << "Animal Destroyed!" << std::endl;
}

int Animal::GetAge() const
{
	return mAge;
}

void Animal::SetAge(int age)
{
	mAge = age;
}

int Animal::GetWeight() const
{
	return mWeight;
}

void Animal::SetWeight(int weight)
{
	mWeight = weight;
}

void Animal::Sound()
{
	std::cout << "Some Noise..." << std::endl;
}
