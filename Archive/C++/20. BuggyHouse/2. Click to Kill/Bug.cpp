#include "Bug.h"

Bug::Bug(D2DFramework* pFramework) :
	Actor(pFramework, L"Data/bug.png")
{
	RECT rct{};
	GetClientRect(pFramework->GetWindowHandle(), &rct);
	mX = static_cast<float>(rand() % rct.right);
	mY = static_cast<float>(rand() % rct.bottom);
	mIsDeleted = false;
}

void Bug::Draw()
{
	if (mMoveCount++ > 15)
	{
		mMoveCount = 0;
		mRotation += (1 - rand() % 3) * 45.f;
	}
	
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rct{
		0, 0,
		static_cast<float>(0 + size.width),
		static_cast<float>(0 + size.height)
	};

	auto pos = UPVECTOR * D2D1::Matrix3x2F::Rotation(mRotation);

	mX += pos.x;
	mY += pos.y;
	
	auto matTranslate = D2D1::Matrix3x2F::Translation(mX, mY);
	auto matRotation = D2D1::Matrix3x2F::Rotation(mRotation, D2D1_POINT_2F{ size.width * 0.5f, size.height * 0.5f });

	auto pRT = mpFramework->GetRenderTarget();

	pRT->SetTransform(matRotation * matTranslate);
	pRT->DrawBitmap(
		mpBitmap,
		rct,
		mOpacity
	);
}

bool Bug::IsClicked(POINT& pt)
{
	auto size{ mpBitmap->GetPixelSize() };
	D2D1_RECT_F rct{
		mX, mY,
		static_cast<float>(mX + size.width),
		static_cast<float>(mY + size.height)
	};

	if ((pt.x >= rct.left && pt.x <= rct.right) &&
		(pt.y >= rct.top && pt.y <= rct.bottom))
	{
		mIsDeleted = true;
	}
	
	return mIsDeleted;
}
