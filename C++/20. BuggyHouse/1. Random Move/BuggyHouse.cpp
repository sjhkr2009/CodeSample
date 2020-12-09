#include "BuggyHouse.h"
#include <random>

HRESULT BuggyHouse::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
	ThrowIfFailed(hr, "BuggyHouse::Initialize() failed");

	mspBackground = std::make_unique<Actor>(dynamic_cast<D2DFramework*>(this), L"Data/background.png",0.f, 0.f);

	int x, y;
	RECT rct{};
	GetClientRect(mHwnd, &rct);

	for (int i = 0; i < 40; i++)
	{
		auto spBug = std::make_unique<Bug>( dynamic_cast<D2DFramework*>(this) );
		mBugList.push_back(std::move(spBug));
	}
	
	return hr;
}

void BuggyHouse::Release()
{
	for (auto& bug : mBugList)
	{
		bug.reset();
	}
	mBugList.clear();
	mspBackground.reset();
	D2DFramework::Release();
}

void BuggyHouse::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	// TODO: Draw
	//----------------------------------------------------

	mspBackground->Draw();

	for (auto& bug : mBugList)
	{
		bug->Draw();
	}


	//----------------------------------------------------

	HRESULT hr = mspRenderTarget->EndDraw();

	// D2DERR_RECREATE_TARGET: Device Lost가 발생한 경우. 렌더 타겟을 재생성해야 한다.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
