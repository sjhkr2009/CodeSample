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

	// ������ ���� ���θ� Ȯ���Ѵ�.
	CheckBugs();
	//----------------------------------------------------

	mspBackground->Draw();

	for (auto& bug : mBugList)
	{
		bug->Draw();
	}

	//----------------------------------------------------

	HRESULT hr = mspRenderTarget->EndDraw();

	// D2DERR_RECREATE_TARGET: Device Lost�� �߻��� ���. ���� Ÿ���� ������ؾ� �Ѵ�.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

void BuggyHouse::CheckBugs()
{
	// GetAsyncKeyState : ���ϴ� �ڵ��� Ű ������ �����´�. Ű ������ & ��Ʈ�����Ͽ� ����Ѵ�. ���� ���¿� ������ �ֱ�� �����ϵ��� Async(�񵿱�)�� ���δ�.
	// Ű ������ 4����Ʈ�� ��Ʈ�� �Ǿ� ������, ���� ���´� �ֻ��(�� ����) ��Ʈ, ���� ���´� ù(�� ������) ��Ʈ�� �� �ִ�.
	// ���� ���, ������ ���翡 Ű�� ��� ���� �ִٸ� �ֻ��/���ϴ� ��Ʈ�� ��� 1�� �Ǹ�, 0x8001 �� �ȴ�.
	
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)	// ���콺 ���� ��ư�� Down�ƴ°�? (0x8000: ������ üũ�Ҷ� �� ���Ȱ�, ����� ������ �ִ�)
	{
		POINT pt;
		GetCursorPos(&pt);	// ���콺 Ŀ�� ��ġ�� �޾ƿ´�. ��, ȭ�� ��ü�� ���� ����� �������� �ϴ� ��ũ�� ��ǥ���̹Ƿ� ����.
		ScreenToClient(mHwnd, &pt); // ��ũ�� ��ǥ�踦 ���� ���α׷��� ���� ����� �������� �ϴ� Ŭ���̾�Ʈ ��ǥ��� �����Ѵ�.

		auto itr = std::remove_if(mBugList.begin(), mBugList.end(), 
			[&](auto& spBug)
			{
				Bug* pBug = spBug.get();
				pBug->IsClicked(pt);

				return pBug->mIsDeleted;
			}
		);
		mBugList.erase(itr, mBugList.end());

		// ������: ����ȭ
		// Draw()���� remove_if()���� ���� ����Ʈ�� ��ȸ�Ͽ�, �� �� ��ȸ�ϰ� �ȴ�.
		
	}
}
