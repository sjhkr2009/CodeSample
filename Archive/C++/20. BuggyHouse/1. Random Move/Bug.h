#pragma once
#include "Actor.h"
class Bug :
    public Actor
{
private:
	enum class Direction
	{
		UP = 0,
		UP_RIGHT = 1,
		RIGHT = 2,
		DOWN_RIGHT = 3,
		DOWN = 4,
		DOWN_LEFT = 5,
		LEFT = 6,
		UP_LEFT = 7,
		
		COUNT = 8,
		STOP = 9
	};
	int deltaX[9]{ 0, 1, 1, 1, 0, -1, -1, -1, 0 };
	int deltaY[9]{ -1, -1, 0, 1, 1, 1, 0, -1, 0 };
	Bug::Direction mDirection;
	int moveCount;

public:
	Bug(D2DFramework* pFramework);

	virtual void Draw() override;
	void SetRandomDir() { mDirection = DirClamp(ToInt(mDirection) + (rand() % 3 - 1)); }

private:
	int ToInt(Bug::Direction dir) { return static_cast<int>(dir); }
	Bug::Direction ToDir(int value) { return static_cast<Bug::Direction>(value); }
	Bug::Direction DirClamp(Bug::Direction dir) { return ToDir((ToInt(dir) + ToInt(Bug::Direction::COUNT)) % ToInt(Bug::Direction::COUNT)); }
	Bug::Direction DirClamp(int dir) { return DirClamp(ToDir(dir)); }
};

