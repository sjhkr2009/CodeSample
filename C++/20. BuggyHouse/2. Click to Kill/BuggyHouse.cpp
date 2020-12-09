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

	mspRenderTarget->SetTransform(D2D1::IdentityMatrix());

	// TODO: Draw

	// 벌레의 생존 여부를 확인한다.
	CheckBugs();
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

void BuggyHouse::CheckBugs()
{
	// GetAsyncKeyState : 원하는 코드의 키 정보를 가져온다. 키 정보와 & 비트연산하여 사용한다. 게임 상태와 별도의 주기로 동작하도록 Async(비동기)를 붙인다.
	// 키 정보는 4바이트의 비트로 되어 있으며, 현재 상태는 최상단(맨 왼쪽) 비트, 이전 상태는 첫(맨 오른쪽) 비트에 들어가 있다.
	// 예를 들어, 이전과 현재에 키가 모두 눌려 있다면 최상단/최하단 비트가 모두 1이 되며, 0x8001 이 된다.
	
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)	// 마우스 왼쪽 버튼이 Down됐는가? (0x8000: 이전에 체크할땐 안 눌렸고, 현재는 눌려져 있다)
	{
		POINT pt;
		GetCursorPos(&pt);	// 마우스 커서 위치를 받아온다. 단, 화면 전체의 좌측 상단을 원점으로 하는 스크린 좌표계이므로 주의.
		ScreenToClient(mHwnd, &pt); // 스크린 좌표계를 현재 프로그램의 좌측 상단을 원점으로 하는 클라이언트 좌표계로 변경한다.

		auto itr = std::remove_if(mBugList.begin(), mBugList.end(), 
			[&](auto& spBug)
			{
				Bug* pBug = spBug.get();
				pBug->IsClicked(pt);

				return pBug->mIsDeleted;
			}
		);
		mBugList.erase(itr, mBugList.end());

		// 문제점: 최적화
		// Draw()때와 remove_if()에서 각각 리스트를 순회하여, 두 번 순회하게 된다.
		
	}
}
