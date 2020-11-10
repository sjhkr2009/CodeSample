#pragma once

#include "Animal.h"

//현재 가장 하위 클래스 (Final Class)

class Dog :
	public Animal
{
public:
	enum BREED
	{
		NONE,
		BEAGLE,
		DOBERMAN,
		BERNARD,
		CHIHUAHUA,
		HUSKY,
		RETRIVER
	};

private:
	BREED mBreed;

public:
	Dog();
	Dog(int age, int weight, Dog::BREED breed);
	~Dog();

	Dog::BREED GetBreed() const;
	void SetBreed(Dog::BREED breed);

	void Rolling();
};

