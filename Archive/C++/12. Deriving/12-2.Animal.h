#pragma once

//현재 가장 상위 클래스 (Root Class)

class Animal
{
// 상속될 요소들을 protected로 잘 설계해야 한다.
protected:
	int mAge;
	int mWeight;

public:
	Animal();
	Animal(int age, int weight);
	~Animal();

	int GetAge() const;
	void SetAge(int age);
	int GetWeight() const;
	void SetWeight(int weight);

	void Sound();
};

