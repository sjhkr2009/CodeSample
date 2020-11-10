#pragma once

#include "Animal.h"

//���� ���� ���� Ŭ���� (Final Class)

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

