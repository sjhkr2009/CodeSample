#include <iostream>

class Character
{
private:
	std::string mName;
	int mHp;
	int mAttack;
	bool mIsDead;

public:
	Character(std::string name, int hp, int attack) :
		mName(name), mHp(hp), mAttack(attack), mIsDead(false)
	{
		std::cout << mName << " 이(가) 생성되었습니다." << std::endl;
	}
	virtual ~Character() 
	{}

public:
	virtual void Attack(Character& target)
	{
		target.Damage(GetPower());
		std::cout << mName << " 이(가) " << target.GetName() << " 을 공격하여 " << GetPower() << "의 피해를 입혔습니다." << std::endl;
	}
	virtual void Damage(int value)
	{
		mHp -= value;

		if (mHp <= 0)
		{
			Dead();
		}
	}
	virtual int GetPower() { return mAttack; }
	int GetAttack() { return mAttack; }
	int GetHp() { return mHp; }
	std::string GetName() { return mName; }
	bool IsDead() { return mIsDead; }

protected:
	virtual void Dead()
	{
		mHp = 0;
		mIsDead = true;
		std::cout << mName << " 이(가) 처치되었습니다!" << std::endl;
	}
};

class Player : public Character
{
private:
	int mWeaponPower;

public:
	Player(std::string name, int hp, int attack, int weaponPower) : Character(name, hp, attack),
		mWeaponPower(weaponPower)
	{
		std::cout << name << " 은 플레이어입니다." << std::endl;
	}
	virtual ~Player() {}

public:
	void Damage(int value) override
	{
		Character::Damage(value);
		std::cout << "피해를 입었습니다. (" << GetName() << "의 남은 체력 : " << GetHp() << ")" << std::endl;
	}
	int GetPower() override { return GetAttack() + mWeaponPower; }

protected:
	void Dead() override
	{
		std::cout << GetName() << " 이(가) 사망했습니다." << std::endl;
	}
};

int main()
{
	Player* pHero = new Player("Player", 50, 10, 5);
	Character* pDragon = new Character("Dragon", 30, 5);

	while (!pDragon->IsDead())
	{
		pHero->Attack(*pDragon);
		pDragon->Attack(*pHero);
	}
}