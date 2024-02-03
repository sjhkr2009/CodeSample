#pragma once

//���� ���� ���� Ŭ���� (Root Class)

class Animal
{
// ��ӵ� ��ҵ��� protected�� �� �����ؾ� �Ѵ�.
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

