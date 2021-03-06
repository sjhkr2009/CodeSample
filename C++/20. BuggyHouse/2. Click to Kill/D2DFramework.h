#pragma once

// 윈도우 프로그래밍에서 Direct 2D 사용 시 사용하는 프레임워크

// D2D Factory 생성 및 초기화: 
// Render Target 생성

#include <d2d1.h>
#include <wrl/client.h>
#include "ComException.h"

class D2DFramework
{
	// 참고: 사용자 정의 생성자가 없거나, 생성자에서 값을 초기화하지 않는 경우 유니폼 초기화식을 사용한다.
private:
	const wchar_t* WindowClassName{ L"MyWindowClass" };

protected:
	HWND mHwnd{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT	InitD2D(HWND hwnd);
	virtual HRESULT CreateDeviceResources();

public:
	// 콜백 함수는 인스턴스 없이도 운영체제가 호출할 수 있도록, public 정적 함수로 생성해야 한다.
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT	Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768);
	virtual void	Release();
	virtual int		GameLoop();
	virtual void	Render();

	ID2D1HwndRenderTarget* GetRenderTarget() const { return mspRenderTarget.Get(); }
	HWND GetWindowHandle() const { return mHwnd; }

private:
	void ErrorBox(LPCWSTR errorText) { MessageBox(nullptr, errorText, L"Error", MB_OK); }
};

