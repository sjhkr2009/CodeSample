#pragma once
#include "Actor.h"
class Bug :
    public Actor
{
private:
	const D2D_POINT_2F UPVECTOR{ 0.0f, -1.0f };
	int mMoveCount{};
	float mRotation{};
	
public:
	bool mIsDeleted;

public:
	Bug(D2DFramework* pFramework);

	virtual void Draw() override;

	bool IsClicked(POINT& pt);
};

