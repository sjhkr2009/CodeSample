#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) :
	Actor(pFramework, L"Data/bug.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);
	mX = static_cast<float>(rand() % rct.right);
	mY = static_cast<float>(rand() % rct.bottom);
	mDirection = ToDir(rand() % ToInt(Bug::Direction::COUNT));
	moveCount = 0;
}

void Bug::Draw()
{
	mX += deltaX[ToInt(mDirection)];
	mY += deltaY[ToInt(mDirection)];
	
	if (moveCount++ > 10)
	{
		moveCount = 0;
		SetRandomDir();
	}
	
	Actor::Draw();
}
