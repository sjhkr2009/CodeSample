#pragma once

// 윈도우 프로그래밍에서 Direct 2D 사용 시 사용하는 프레임워크

// D2D Factory 생성 및 초기화: 
// Render Target 생성

#include <d2d1.h>
#include <wrl.h>

class D2DFramework
{
	// 참고: 사용자 정의 생성자가 없거나, 생성자에서 값을 초기화하지 않는 경우 유니폼 초기화식을 사용한다.

protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspD2DRenderTarget{};

public:
	virtual HRESULT	Initialize(HWND hwnd);
	virtual void	Release();
	virtual void	Render();

private:
	void ShowErrorMsg(HRESULT errCode, LPCWSTR errMsg, LPCWSTR errTitle = L"Error");
};

