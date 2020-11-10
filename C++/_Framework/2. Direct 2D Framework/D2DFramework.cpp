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
	
	// SetWindowLongPtr:	�����츦 ������ �Ŀ��� �ڵ��� ���� Ȯ�� ��Ÿ��, ����� ���� ������, �޽��� ���ν��� ���� ������ �� �ִ�.
	//						���⼭�� ���� �������� ���� �ν��Ͻ��� �����ϴ� �� ����Ѵ�.
	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_NORMAL);
	UpdateWindow(mHwnd);

	return S_OK;
}

LRESULT CALLBACK D2DFramework::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ���� �Լ������� �ν��Ͻ��� ���ӵ� ��� ������ �Լ��� ����� �� ����.
	// ���� ������ ���� �� �����ص� �ν��Ͻ��� �����͸� �ҷ��� ����Ѵ�.
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
	GetClientRect(mHwnd, &wr); // Ŭ���̾�Ʈ ������ ���ؼ� wr�� �����ϴ� �Լ�.
	
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

	// D2DERR_RECREATE_TARGET: Device Lost�� �߻��� ���. ���� Ÿ���� ������ؾ� �Ѵ�.
	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}
