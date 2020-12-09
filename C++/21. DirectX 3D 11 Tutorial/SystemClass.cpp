#include "SystemClass.h"

SystemClass::SystemClass() :
	mInput{ nullptr }, mGraphics{ nullptr } {}

void SystemClass::InitializeWindows(int& width, int& height)
{
	// ����� Ŭ������, ��üȭ���� ��� ��ũ�� ũ�⸦ ������ Ŭ����, â����� ��� â ��ġ�� ������ x,y������ �����Ѵ�.
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Ŭ������ ������ ������ �޸� �ʱ�ȭ
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// ���α׷��� �ڵ��� �� Ŭ������ �����ϰ�, �ν��Ͻ��� ������ �� �̸��� �����Ѵ�.
	ApplicationHandle = this;

	mHinstance = GetModuleHandle(NULL);
	mApplicationName = L"DirectX Test";

	// Ŭ���� ���� �� ���
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

	// ���� �ý����� �ػ󵵸� �����´�.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ ��ũ�� ����� ��� �ػ󵵿� ���� â ũ�⸦ �����ϰ�, 32��Ʈ�� ����
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
	// â ����� ��� �ϴ� 800 x 600 ũ��� �����ϰ�, ȭ�� �߾ӿ� ��ġ�Ѵ�.
	else
	{
		height = 800;
		width = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// ������ â�� �����Ѵ�.
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

	// ������ â�� ���� �� â�� �� �տ� �� �� ��Ŀ���Ѵ�.
	ShowWindow(mHwnd, SW_SHOW);
	SetForegroundWindow(mHwnd);
	SetFocus(mHwnd);

	// ���콺 Ŀ���� �� ���̰� ó��
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ�� ���̰�
	ShowCursor(true);

	// TODO : if(FULL_SCREEN)
	
	// ������ â ����
	DestroyWindow(mHwnd);
	mHwnd = nullptr;

	// ��ϵ� Ŭ���� ����
	UnregisterClass(mApplicationName, mHinstance);
	mHinstance = nullptr;

	// �ڵ� �ʱ�ȭ
	ApplicationHandle = nullptr;
}

bool SystemClass::Init()
{
	int screenWidth{}, screenHeight{};

	// ������API�� �ʱ�ȭ�Ѵ�. ������ �Ű������� �����Ƿ� ���� �� ����� screenWidth, screenHeight�� �� ���̴�.
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
	 
	// �޽��� ����ü�� �޸� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	// done�� true�� �ƴ� �� �ݺ��Ѵ�.
	while (!done)
	{
		// �޽����� ������ ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// ���� �޽����� done�� true�� �ٲٰ�, �ƴ� ��� Frame()�� �����Ѵ�.
		// Frame() ���࿡ �����Ͽ� false�� ��ȯ�Ǹ� ���������� done�� true�� �ٲ۴�.
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
