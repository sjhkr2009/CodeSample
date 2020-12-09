#include "D3DFramework.h"

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	mInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = mClassName.c_str();
	wc.hInstance = mInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register!", L"Error", MB_OK);
		return;
	}

	RECT wr{ 0, 0, mScreenWidth, mScreenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindowEx(
		NULL,
		mClassName.c_str(),
		mTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		mInstance,
		NULL
	);

	if (mHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return;
	}

	// ������ ���ν����� ���� �Լ��̹Ƿ� ����� ������ �Ұ����ϴ�.
	// �ʿ��� ������ �����쿡�� �޾ƿ� �� �ֵ��� ������ �����͸� �����Ѵ�.
	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_SHOW);	// ������ ��κ� ���� �� ȭ�鿡 ���;� �ϹǷ� SW_SHOW�� �����ȴ�. (�ٸ� �ɼ��� Ʈ���� �����ܸ� �����ϴ� ���� Ŀ�ǵ�)
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
	UpdateWindow(mHwnd);
}

void D3DFramework::InitD3D()
{
	// 1) ���� ü�� ����
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = mHwnd;
	scd.Windowed = TRUE;

	scd.SampleDesc.Count = 1;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.BufferDesc.Width = mScreenWidth;
	scd.BufferDesc.Height = mScreenHeight;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&scd,
		mspSwapChain.GetAddressOf(),
		mspDevice.GetAddressOf(),
		NULL,
		mspDeviceContext.GetAddressOf()
	);

	// 2) ~ 5) OnResize ���� ó��.
	OnResize();
}

void D3DFramework::OnResize()
{
	// (���� ������ �ƴ� ���) �ػ� ���� �� ����� ��ҵ��� ���, ������ �ʱ�ȭ
	mspDepthStencil.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspRenderTargetView.Reset();
	mspDeviceContext->Flush();

	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mspDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	mspSwapChain->ResizeBuffers(0, mScreenWidth, mScreenHeight, DXGI_FORMAT_UNKNOWN, 0);

	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// 2) ���� Ÿ�� ����.
	mspSwapChain->GetBuffer(0, IID_PPV_ARGS(mspRenderTarget.GetAddressOf()));
	mspDevice->CreateRenderTargetView(mspRenderTarget.Get(), nullptr, mspRenderTargetView.GetAddressOf());


	// 3) ����-���ٽ� ����
	CD3D11_TEXTURE2D_DESC dtd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		mScreenWidth, mScreenHeight,
		1U, 1U,	
		D3D11_BIND_DEPTH_STENCIL
	);
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

	mspDevice->CreateTexture2D(&dtd, nullptr, mspDepthStencil.GetAddressOf());
	mspDevice->CreateDepthStencilView(mspDepthStencil.Get(), &dsvd, mspDepthStencilView.GetAddressOf());


	// 4) �ĸ� ���ۿ� ����-���ٽ� ���� ��ġ��.
	mspDeviceContext->OMSetRenderTargets(
		1,
		mspRenderTargetView.GetAddressOf(),
		mspDepthStencilView.Get()
	);

	// 5) �� ��Ʈ ����.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(mScreenWidth), static_cast<float>(mScreenHeight));
	mspDeviceContext->RSSetViewports(1, &viewport);
}

void D3DFramework::RenderFrame()
{
	float clearColor[4]{ 0.f, 0.2f, 0.4f, 1.f };
	mspDeviceContext->ClearRenderTargetView(mspRenderTargetView.Get(), clearColor);
	mspDeviceContext->ClearDepthStencilView(mspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	// �׸��� �κ�
	Render();

	mspSwapChain->Present(0, 0);
}

void D3DFramework::Initialize(HINSTANCE hInstance, int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	InitWindow(hInstance);
	InitD3D();
}

void D3DFramework::Destroy()
{
	mspSwapChain->SetFullscreenState(FALSE, nullptr);

	mspDepthStencil.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspRenderTargetView.Reset();

	mspSwapChain.Reset();
	mspDevice.Reset();
	mspDeviceContext.Reset();

	DestroyWindow(mHwnd);
	UnregisterClass(mClassName.c_str(), mInstance);
}

void D3DFramework::GameLoop()
{
	MSG msg{};
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
			RenderFrame();
		}
	}
}

LRESULT D3DFramework::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			if (mResizing)
			{
				RenderFrame();
			}
			else
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
			break;

		case WM_SIZE:
			mScreenWidth = LOWORD(lParam);
			mScreenHeight = HIWORD(lParam);
			if (mspDevice)
			{
				if (wParam == SIZE_MINIMIZED)
				{
					mMinimized = true;
					mMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					mMinimized = false;
					mMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (mMinimized)
					{
						mMinimized = false;
						OnResize();
					}
					else if (mMaximized)
					{
						mMaximized = false;
						OnResize();
					}
					else if (mResizing)
					{
					}
					else
					{
						OnResize();
					}
				}
			}
			return 0;
			break;

		case WM_ENTERSIZEMOVE:
			mResizing = true;
			return 0;
			break;

		case WM_EXITSIZEMOVE:
			mResizing = false;
			OnResize();
			return 0;
			break;

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
			break;

		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
			break;

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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	D3DFramework* pFramework = reinterpret_cast<D3DFramework*>(
		GetWindowLongPtr(hwnd, GWLP_USERDATA)
		);

	return pFramework->MessageHandler(hwnd, msg, wParam, lParam);
}