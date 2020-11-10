#include <iostream>

// 다형성 활용과 가상함수 (Polymorphism / Virtual Function)

class Character
{
public:
	void Attack() { std::cout << "CHARACTER::ATTACK()" << std::endl; }
	virtual void Attack2() { std::cout << "CHARACTER::ATTACK()" << std::endl; }
};

class Warrior : public Character
{
public:
	void Attack() { std::cout << "Warrior::ATTACK()" << std::endl; }
	void Attack2() { std::cout << "Warrior::ATTACK()" << std::endl; }
};

class Archor : public Character
{
public:
	void Attack() { std::cout << "Archor::ATTACK()" << std::endl; }
	void Attack2() override // 모던 C++에서는 가상함수임을 알려주기 위해, 자식 클래스에도 override 키워드를 써서 쌍을 맞춘다.
	{ std::cout << "Warrior::ATTACK()" << std::endl; }
};

// PVP 게임이라고 가정할 때, 충돌 처리 함수

// 부모 클래스를 이용하면, 궁수와 전사, 전사와 전사, 궁수와 궁수 등의 경우의 수에 대해 일일이 함수를 만들 필요가 없다.
// 단, 복사형으로 쓰면 내부적으로 부모 클래스를 하나 더 만드는 것이나 다름없으며, 참조형 변수로 넘겨야 한다.
bool CheckCollision(Character* c1, Character* c2) { return false; }

int main()
{
	Character* pWarrior = new Warrior();

	pWarrior->Attack(); // Warrior로 생성했지만, Character로 캐스팅했으므로 부모 클래스의 함수가 호출된다.
	// 이를 방지하고, Warrior로 생성된 객체가 항상 Warrior의 함수를 호출하게 하려면 virtual function(가상 함수)을 사용한다.

	pWarrior->Attack2();
	// 가상함수를 오버라이딩했으므로 부모 클래스의 함수를 호출했지만 자식 클래스에 오버라이딩된 함수가 호출된다.
}