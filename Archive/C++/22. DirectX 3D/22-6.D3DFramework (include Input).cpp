#include "D3DFramework.h"
#include <sstream>

void D3DFramework::InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	mInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = CLASS_NAME.c_str();
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

	mTitleText = TITLE_NAME;

	mHwnd = CreateWindowEx(
		NULL,
		CLASS_NAME.c_str(),
		mTitleText.c_str(),
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

	SetWindowLongPtr(mHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);
	UpdateWindow(mHwnd);
}

void D3DFramework::InitD3D()
{
	// 1) 스왑 체인 생성
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

	// 2) ~ 5) OnResize 에서 처리.
	OnResize();
}

void D3DFramework::CalculateFPS()
{
	static int frameCount{ 0 };
	static float timeElapsed{ 0.f };

	frameCount++;

	if (mTimer.TotalTime() - timeElapsed >= 1.f)
	{
		float fps = (float)frameCount;
		float msfp = 1000.f / fps;

		std::wostringstream outs;
		outs.precision(4);
		outs << mTitleText << L" - " <<
			L"FPS: " << fps << L", Frame Time: " << msfp << L"(ms)";

		SetWindowText(mHwnd, outs.str().c_str());
		
		frameCount = 0;
		timeElapsed += 1.f;
	}
}

void D3DFramework::OnResize()
{
	// (최초 실행이 아닐 경우) 해상도 변경 시 변경될 요소들의 경우, 기존값 초기화
	mspDepthStencil.Reset();
	mspDepthStencilView.Reset();
	mspRenderTarget.Reset();
	mspRenderTargetView.Reset();
	mspDeviceContext->Flush();

	ID3D11RenderTargetView* nullViews[] = { nullptr };
	mspDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	mspSwapChain->ResizeBuffers(0, mScreenWidth, mScreenHeight, DXGI_FORMAT_UNKNOWN, 0);

	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// 2) 렌더 타겟 설정.
	mspSwapChain->GetBuffer(0, IID_PPV_ARGS(mspRenderTarget.GetAddressOf()));
	mspDevice->CreateRenderTargetView(mspRenderTarget.Get(), nullptr, mspRenderTargetView.GetAddressOf());


	// 3) 깊이-스텐실 버퍼
	CD3D11_TEXTURE2D_DESC dtd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		mScreenWidth, mScreenHeight,
		1U, 1U,	
		D3D11_BIND_DEPTH_STENCIL
	);
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

	mspDevice->CreateTexture2D(&dtd, nullptr, mspDepthStencil.GetAddressOf());
	mspDevice->CreateDepthStencilView(mspDepthStencil.Get(), &dsvd, mspDepthStencilView.GetAddressOf());


	// 4) 후면 버퍼와 깊이-스텐실 버퍼 합치기.
	mspDeviceContext->OMSetRenderTargets(
		1,
		mspRenderTargetView.GetAddressOf(),
		mspDepthStencilView.Get()
	);

	// 5) 뷰 포트 설정.
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

	// 그리는 부분
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
	UnregisterClass(CLASS_NAME.c_str(), mInstance);
}

void D3DFramework::GameLoop()
{
	// 게임 루프 시작할 때 초기화
	mTimer.Start();
	mInput.Initialize();
	
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
			mTimer.Update();
			
			if (mPaused)
			{
				Sleep(50);
				continue;
			}

			CalculateFPS();
			Update(mTimer.DeltaTime());
			RenderFrame();
		}
	}
}

LRESULT D3DFramework::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Input 관련 메시지
		//-------------------------------------------------------------------
		// 키를 누르거나 뗐을 때. wParam에 누른 키의 번호가 들어온다.
		case WM_KEYDOWN:
			mInput.SetKeyDown(wParam);
			break;
		case WM_KEYUP:
			mInput.SetKeyUp(wParam);
			break;

		// 마우스 입력은 별도로 처리한다.
		case WM_LBUTTONDOWN:
			mInput.SetKeyDown(VK_LBUTTON);
			break;
		case WM_NCLBUTTONUP:
			mInput.SetKeyUp(VK_LBUTTON);
			break;

		// 마우스 이동 시 들어오는 메시지. lParam의 Low/High Word에 x,y 좌표가 들어온다.
		case WM_MOUSEMOVE:
			mInput.SetCursor(LOWORD(lParam), HIWORD(lParam));
			break;

		//-------------------------------------------------------------------

		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				mPaused = true;
				mTimer.Stop();
			}
			else
			{
				mPaused = false;
				mTimer.Resume();
			}

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
					if (!mPaused)
					{
						mTimer.Stop();
					}
					mPaused = true;
					mMinimized = true;
					mMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					mTimer.Resume();
					mPaused = false;
					mMinimized = false;
					mMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (mMinimized)
					{
						mTimer.Resume();
						mPaused = false;
						mMinimized = false;
						OnResize();
					}
					else if (mMaximized)
					{
						mTimer.Resume();
						mPaused = false;
						mMaximized = false;
						OnResize();
					}
					else if (mResizing)
					{
					}
					else
					{
						mTimer.Resume();
						mPaused = false;
						OnResize();
					}
				}
			}
			return 0;
			break;

		case WM_ENTERSIZEMOVE:
			mPaused = true;
			mTimer.Stop();
			mResizing = true;
			return 0;
			break;

		case WM_EXITSIZEMOVE:
			mTimer.Resume();
			mPaused = false;
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
