#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = WindowClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	if (!RegisterClassEx(&wc))
	{
		ErrorBox(L"Failed to Register!");
		return 1;
	}

	RECT wr{ 0,0,
		static_cast<LONG>(width),
		static_cast<LONG>(height)
	};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindowEx(
		NULL,
		WindowClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(wr.right - wr.left), (wr.bottom - wr.top),
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (mHwnd == nullptr)
	{
		ErrorBox(L"Failed to Create Window!");
		return 1;
	}
	
	// SetWindowLongPtr:	윈도우를 생성한 후에도 핸들을 통해 확장 스타일, 사용자 임의 데이터, 메시지 프로시저 등을 제어할 수 있다.
	//						여기서는 임의 데이터인 현재 인스턴스를 저장하는 데 사용한다.
	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_NORMAL);
	UpdateWindow(mHwnd);

	return S_OK;
}

LRESULT CALLBACK D2DFramework::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 정적 함수에서는 인스턴스에 종속된 멤버 변수나 함수를 사용할 수 없다.
	// 따라서 윈도우 생성 때 저장해둔 인스턴스의 포인터를 불러와 사용한다.
	D2DFramework* pFramework = reinterpret_cast<D2DFramework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

HRESULT D2DFramework::InitD2D(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mspFactory.GetAddressOf());
	ThrowIfFailed(hr);
	
	hr = CreateDeviceResources();

	return S_OK;
}

HRESULT D2DFramework::CreateDeviceResources()
{
	RECT wr;
	GetClientRect(mHwnd, &wr); // 클라이언트 영역을 구해서 wr에 배정하는 함수.
	
	HRESULT hr = mspFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			mHwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		mspRenderTarget.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);
	return hr;
}

HRESULT D2DFramework::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	HRESULT hr;

	hr = InitWindow(hInstance, title, width, height);
	ThrowIfFailed(hr);

	hr = InitD2D(mHwnd);
	ThrowIfFailed(hr);

	return hr;
}

void D2DFramework::Release()
{
	mspRenderTarget.Reset();
	mspFactory.Reset();
}

int D2DFramework::GameLoop()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			Render();
		}
	}

	return static_cast<int>(msg.wParam);
}

void D2DFramework::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	// TODO: Draw

	HRESULT hr = mspRenderTarget->EndDraw();

	// D2DERR_RECREATE_TARGET: Device Lost가 발생한 경우. 렌더 타겟을 재생성해야 한다.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
