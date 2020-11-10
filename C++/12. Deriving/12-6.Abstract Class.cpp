#include <iostream>

// 추상 클래스 (Abstract Class) : 순수 가상함수가 있는 클래스.
// 상속 전용으로, 인스턴스화할 수 없다. 자식 클래스는 반드시 순수 가상함수의 내용을 정의해야 한다.
// 일종의 인터페이스(공통 규약)의 역할을 한다.
class Character
{
protected:
	int mHp;

public:
	Character() : mHp{} {}
	virtual ~Character() {}

public:
	// 순수 가상함수(pure virtual function): 'virtual 함수명 = 0' 의 형태로 만든다.
	// 이런 함수는 자식 클래스에서 반드시 오버라이딩해야 한다.
	virtual void Attack() = 0;
	virtual void Dead() = 0;
};

class Warrior : public Character
{
public:
	Warrior() {};
	~Warrior() {};

public:
	void Attack() {}
	void Dead() {}

	void DoubleBash() {}
};

int main()
{
	Warrior w;

	// 추상 클래스는 인스턴스화할 수는 없으나, 포인터로 사용할수는 있다.
	Character* party[] = { new Warrior(), new Warrior() };

	w.Attack();
	w.DoubleBash();
}