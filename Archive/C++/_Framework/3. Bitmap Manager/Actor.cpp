#include "Actor.h"
#include "BitmapManager.h"

using namespace Microsoft::WRL;

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename) :
	mpFramework{ pFramework }, mX{}, mY{}, mOpacity{ 1.0f }
{
	mpBitmap = BitmapManager::Instance().LoadBitmap(filename);
}

Actor::Actor(D2DFramework* pFramework, LPCWSTR filename, float x, float y, float opacity) :
	Actor(pFramework, filename)
{
	mX = x;
	mY = y;
	mOpacity = opacity;
}

Actor::~Actor()
{
	mpBitmap = nullptr;
}

void Actor::Draw()
{
	auto rt = mpFramework->GetRenderTarget();
	auto size = mpBitmap->GetPixelSize();

	rt->DrawBitmap(mpBitmap,
		D2D1_RECT_F{ mX, mY, static_cast<float>(mX + size.width), static_cast<float>(mY + size.height) },
		mOpacity
	);
}
