#include <iostream>

// 다형성 활용 3 (Polymorphism 3)

class Character
{
public:
	// 생성자는 무조건 부모 생성 후 자식 생성이 진행되므로, 가상 생성자는 존재하지 않는다.
	Character() { std::cout << "Character Created!" << std::endl; }
	
	// 기반 클래스의 소멸자는 가상으로 선언한다.
	// 그렇지 않으면 부모 클래스로 캐스팅된 인스턴스를 삭제할 시, 자식 클래스에 할당된 메모리가 해제되지 않는다.
	//		(virtual 삭제 시 자식 클래스의 소멸자 호출되지 않음)
	virtual ~Character() { std::cout << "Character Destroyed!" << std::endl; }

	virtual Character* Clone() const { return new Character(*this); }

	virtual void Attack() { std::cout << "CHARACTER::ATTACK()" << std::endl; }
};


class Warrior : public Character
{
public:
	Warrior() { std::cout << "Warrior Created!" << std::endl; }
	~Warrior() { std::cout << "Warrior Destroyed!" << std::endl; }

	void Attack() override
	{ 
		std::cout << "\tWarrior::ATTACK()" << std::endl; 
	}
	void DoubleBash()
	{ 
		std::cout << "\tWarrior::DoubleBash()" << std::endl; 
	}
};

class Archer : public Character
{
public:
	Archer() { std::cout << "Archor Created!" << std::endl; }
	~Archer() { std::cout << "Archor Destroyed!" << std::endl; }

	void Attack() override
	{
		std::cout << "\tArchor::ATTACK()" << std::endl; 
	}
	void TrueShot() 
	{
		std::cout << "\tArchor::TrueShot()" << std::endl;
	}
};


int main()
{
	Character* party[] = { new Warrior(), new Archer() }; // 암시적으로 기반 클래스로 변환

	for (int i = 0; i < 2; i++)
	{
		dynamic_cast<Warrior*>(party[i])->DoubleBash(); // 명시적으로 파생 클래스로 변환
	}
}