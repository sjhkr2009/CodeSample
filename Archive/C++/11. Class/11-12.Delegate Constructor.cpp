#include <iostream>

// 대리 생성자 (Delegate Constructor)

class Player
{
public:
	int mHp;
	int mMp;

public:
	Player(int hp, int mp)
	{
		mHp = hp;
		mMp = mp;
	}
	
	// Hp, Mp 입력이 없을 때 0으로 하려면, 다른 생성자를 불러와서 초기화시킬 수 있다.
	Player() : Player(0, 0)
	{}

	// 여러 형태의 생성자가 필요할 때 이렇게 대리 생성자를 이용하면 간편한 표기가 가능하다.
	Player(int hp) : Player(hp, 0)
	{}
};

int main()
{
	
}