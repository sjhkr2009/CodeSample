#include <iostream>

// 친구(friend)

// 친구인 클래스는 private, protected 멤버에 접근이 가능하다.

class Sword
{
	// Warrior를 친구로 등록한다.
	friend class Warrior;

private:
	int mDamage;
public:
	Sword(int damage) : mDamage{ damage } {}

	// 친구 함수를 등록할 경우, 해당 함수 내에서 이 클래스의 private/protected 변수나 함수에 접근할 수 있다.
	friend void Enhance(Sword& sword);

	// 실제로 friend를 활용하는 경우는 주로 연산자 오버로딩.
	// 출력을 담당하는 std::ostream에서 Sword 클래스에 접근하여 mDamage 변수를 출력할 수 있게 만든다.
	friend std::ostream& operator << (std::ostream& os, const Sword sword);
};

// 전역 함수로 생성되지만, Sword 내의 private/protected 변수나 함수에 접근할 수 있다.
void Enhance(Sword& sword)
{
	sword.mDamage *= 2;

	std::cout << "칼을 강화했다!" << std::endl;
}

std::ostream& operator << (std::ostream& os, const Sword sword)
{
	os << "검 (데미지: " << sword.mDamage << ")" << std::endl;
	return os;
}

class Warrior
{
public:
	void AttackWith(Sword& sword)
	{
		// Warrior는 friend로 등록된 클래스이므로, private 변수인 mDamage에 접근할 수 있다.
		std::cout << "칼을 휘둘러서 " << sword.mDamage << "의 피해를 주었다!" << std::endl;

		// 다만, 이런 경우 friend를 쓰기보단 주로 Getter 함수를 만들어 사용한다.
	}
};

int main()
{
	Sword shortSword(10);
	Warrior w;

	std::cout << shortSword;

	w.AttackWith(shortSword);

	Enhance(shortSword);
	w.AttackWith(shortSword);
}
