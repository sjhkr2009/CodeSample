#include "SystemClass.h"

SystemClass::SystemClass() :
	mInput{ nullptr }, mGraphics{ nullptr } {}

void SystemClass::InitializeWindows(int& width, int& height)
{
	// 등록할 클래스와, 전체화면일 경우 스크린 크기를 제어할 클래스, 창모드일 경우 창 위치를 제어할 x,y변수를 선언한다.
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 클래스를 생성할 공간의 메모리 초기화
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// 프로그램의 핸들을 이 클래스로 지정하고, 인스턴스를 가져온 후 이름을 지정한다.
	ApplicationHandle = this;

	mHinstance = GetModuleHandle(NULL);
	mApplicationName = L"DirectX Test";

	// 클래스 생성 및 등록
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= NULL;
	wc.cbWndExtra		= NULL;
	wc.hInstance 		= mHinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hIconSm			= wc.hIcon;
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName 	= mApplicationName;
	wc.cbSize 			= sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// 현재 시스템의 해상도를 가져온다.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	// 풀 스크린 모드일 경우 해상도에 따라 창 크기를 조절하고, 32비트로 설정
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long)width;
		dmScreenSettings.dmPelsHeight	= (unsigned long)height;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	// 창 모드일 경우 일단 800 x 600 크기로 생성하고, 화면 중앙에 배치한다.
	else
	{
		height = 800;
		width = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// 윈도우 창을 생성한다.
	mHwnd = CreateWindowExW(
		WS_EX_APPWINDOW,
		mApplicationName,
		mApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY,
		width, height,
		NULL,
		NULL,
		mHinstance,
		NULL
	);

	// 윈도우 창을 띄우고 이 창을 맨 앞에 둔 뒤 포커싱한다.
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	// 마우스 커서를 안 보이게 처리
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	// 마우스 커서 보이게
	ShowCursor(true);

	// TODO : if(FULL_SCREEN)
	
	// 윈도우 창 제거
	DestroyWindow(mHwnd);
	mHwnd = nullptr;

	// 등록된 클래스 해제
	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = nullptr;

	// 핸들 초기화
	ApplicationHandle = nullptr;
}

bool SystemClass::Init()
{
	int screenWidth{}, screenHeight{};

	// 윈도우API를 초기화한다. 참조형 매개변수를 받으므로 실행 후 결과가 screenWidth, screenHeight에 들어갈 것이다.
	InitializeWindows(screenWidth, screenHeight);

	mInput = new InputClass;
	if (!mInput)
	{
		return false;
	}
	mInput->Init();

	mGraphics = new GraphicsClass;
	if (!mGraphics)
	{
		return false;
	}

	bool result;
	result = mGraphics->Init(screenWidth, screenHeight, mHwnd);

	return result;
}

void SystemClass::Shutdown()
{
	if (mGraphics)
	{
		mGraphics->Shutdown();
		delete mGraphics;
		mGraphics = nullptr;
	}

	if (mInput)
	{
		delete mInput;
		mInput = nullptr;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done{ false };
	 
	// 메시지 구조체의 메모리 초기화
	ZeroMemory(&msg, sizeof(MSG));

	// done이 true가 아닌 한 반복한다.
	while (!done)
	{
		// 메시지가 있으면 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// 종료 메시지면 done을 true로 바꾸고, 아닐 경우 Frame()을 실행한다.
		// Frame() 실행에 실패하여 false가 반환되면 마찬가지로 done을 true로 바꾼다.
		done = (msg.message == WM_QUIT) ? true : !Frame();
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			mInput->KeyDown(static_cast<unsigned int>(wParam));
			return 0;
		case WM_KEYUP:
			mInput->KeyDown(static_cast<unsigned int>(wParam));
			return 0;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

}

bool SystemClass::Frame()
{
	bool result;
	
	result = (mInput->IsKeyDown(VK_ESCAPE)) ? false : mGraphics->Frame();
	return result;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY: case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		default:
			ApplicationHandle->MessageHandler(hwnd, msg, wParam, lParam);
	}
}
