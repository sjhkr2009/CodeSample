#include <iostream>

// 다형성 활용 2 (Polymorphism2)

class Character
{
public:
	// 생성자는 무조건 부모 생성 후 자식 생성자가 호출되므로, 가상 생성자는 존재하지 않는다.
	Character() { std::cout << "Character Created!" << std::endl; }

	// 기반 클래스의 소멸자는 가상으로 선언한다.
	// 그렇지 않으면 부모 클래스로 캐스팅된 인스턴스를 삭제할 시, 자식 클래스에 할당된 메모리가 해제되지 않는다.
	//		(가상 소멸자가 아닐 경우, 부모 클래스 포인터로 delete 진행 시 자식 클래스의 소멸자 호출되지 않음)
	virtual ~Character() { std::cout << "Character Destroyed!" << std::endl; }

	// 부모 클래스의 포인터로 자식 클래스 생성자를 호출하려면, 복사 생성자와 동일한 기능의 가상함수를 만드는 방식을 이용한다.
	virtual Character* Clone() const { return new Character(*this); }

	virtual void Attack() { std::cout << "CHARACTER::ATTACK()" << std::endl; }
};


class Warrior : public Character
{
public:
	Warrior() { std::cout << "Warrior Created!" << std::endl; }
	~Warrior() { std::cout << "Warrior Destroyed!" << std::endl; }

	Character* Clone() const { return new Character(*this); }

	void Attack()
	{
		std::cout << "Warrior::ATTACK()" << std::endl;
	}
};

class Archor : public Character
{
public:
	Archor() { std::cout << "Archor Created!" << std::endl; }
	~Archor() { std::cout << "Archor Destroyed!" << std::endl; }

	void Attack()
	{
		std::cout << "Archor::ATTACK()" << std::endl;
	}
};


int main()
{
	// 5인 파티 구성 예시

	Character* party[5] = {
		new Warrior,
		new Archor,
		new Warrior,
		new Archor,
		new Archor	// 참고: 생성자를 안 쓰면 Archor()와 같이 기본 생성자가 호출된다.
	};

	for (int i = 0; i < 5; i++)
	{
		party[i]->Attack();
	}

	for (int i = 0; i < 5; i++)
	{
		delete party[i];
	}

	Character* w1 = new Warrior;

	Character* c2 = new Character(*w1); // w1은 Warrior로 생성되었지만, Character 포인터이므로 부모 클래스의 복사 생성자가 호출된다.
	Character* c3 = w1->Clone(); // 부모 클래스의 Clone이 가상함수이므로, Warrior 클래스의 Clone() 함수가 발동된다.
}