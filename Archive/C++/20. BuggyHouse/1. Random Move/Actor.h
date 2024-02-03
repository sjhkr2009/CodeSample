#pragma once

#include "D2DFramework.h"

class Actor
{
protected:
	D2DFramework* mpFramework;
	ID2D1Bitmap* mpBitmap;

	float mX;
	float mY;
	float mOpacity;

public:
	Actor() = delete;
	Actor(D2DFramework* pFramework, LPCWSTR filename);
	Actor(D2DFramework* pFramework, LPCWSTR filename,
		float x, float y, float opacity = 1.0f);
	virtual ~Actor();

	virtual void Draw();

	void SetPosition(float x, float y) { mX = x; mY = y; }
	void SetPosition(D2D1_POINT_2F pos) { mX = pos.x; mY = pos.y; }
	D2D1_POINT_2F GetPosition() const { return D2D1_POINT_2F{ mX, mY }; }

	void MovePosition(float x, float y) { mX += x; mY += y; }

};