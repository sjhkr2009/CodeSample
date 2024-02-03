// GameLogic.h�� ���� Windows.h �� Gdiplus�� �ҷ����Ƿ�, ������ include�� ���ʿ��ϴ�.
#include "GameLogic.h"

#pragma comment ( lib, "Gdiplus.lib" )

const wchar_t gClassName[]{ L"MyWindowClass" };

// �ָ��׾� ���� ����� (= �����׸� ã��)

// �׽�Ʈ �ڵ�
// ---------------------------------

Solitaire::GameLogic gLogic;

// ---------------------------------

// ����
//
/*
	ī��� ���� �ý����� �������� ��ҵ��� ���� �����, �̸� �����ϴ� �ý����� ����� ����� '����� ����'(Bottom-Up Design)�� �Ѵ�.
	�̿� �ݴ�� ���� �Ŵ����� ���� ū �ý����� ���� ����� ������ ��Ҹ� ����� ����� '����� ����'(Top-Down Design)�� �Ѵ�.
	���ſ� �ӽ� �ڵ�� ������ �׽�Ʈ �ʿ� ���� ��ü �ڵ带 ��� �������� �� �ִ� ���ڸ� ��ȣ������, �ֱٿ� ������ ������ Ŀ�� �������� �׽�Ʈ�� ������ ���ڸ� ��ȣ�Ѵ�.

	����� ���迡���� ������ ��ҵ��� �� �����ϴ��� Ȯ���� �׽�Ʈ�� �ʿ��ϴ�. �̸� Unit Test��� �Ѵ�.
	���� ���ӵ��̳�, ���� ������ �۰� (�ַ� ����޿���) ���谡 �� ¥���� �ִٸ� ����� ����� �� ���� ������ �߱��ϱ⵵ �Ѵ�.

	1�� �����̳� �ε� ������ ������ �׽�Ʈ�� ������ ����� ����� ���൵�ϴ�.

*/ 



// ������ ���ν��� ����
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

using namespace Gdiplus;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)
{
	GdiplusStartupInput gdiInput;
	ULONG_PTR gdiToken;

	GdiplusStartup(&gdiToken, &gdiInput, nullptr);

	// 1. ������ Ŭ���� ���� �� ���
	//	1-1) ������ �ڵ� �� ������ Ŭ������ ���� Ŭ����EX ����
	HWND hWnd;
	WNDCLASSEX wc;

	//	1-2) �޸𸮸� Ư���� ��(=0)���� �ʱ�ȭ
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//	1-3) WNDCLASSEX ����ü�� �⺻������ ä���ش�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//	1-4) �ʱ�ȭ�� ���� Ŭ������ ��� (+ ���� �� ����ó��)
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register!", L"Error", MB_OK);
		return 0;
	}

	// 2. ������ ����
	//	2-1) ������ ũ�� ����
	RECT wr = { 0, 0, 1024, 768 };						// Client Rect. �츮�� �ǵ��ϴ� Ŭ���̾�Ʈ ũ��.
	AdjustWindowRect(&wr, (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION), FALSE);	// Ŭ���̾�Ʈ ũ�⿡ ���ϴ� ��Ÿ���� �������� �� ���� â ũ�Ⱑ ��� �� �� ����Ͽ� wr�� �ݿ����ش�.
	// WS_OVERLAPPEDWINDOW�� ���� �ٸ� â�� ���ų�, �ý��� �޴�, â ũ�������� ������ �ܰ���, �ִ�ȭ, �ּ�ȭ ���� ��Ÿ���� ���ԵǾ� �ִ�.
	// �츮�� â ũ�� ������ �ع����� ��� ũ�Ⱑ �޶����Ƿ�, �������� �ý��� �޴�, ��� Ÿ��Ʋ���ٸ� �������� �����Ѵ�.

	//	2-2) ������ ����
	hWnd = CreateWindowEx(
		NULL,
		gClassName,
		L"SolitaireGame",
		(WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION),
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//	2-3) ���� ó��
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return 0;
	}

	//---------------------------------------------------------------
	
	gLogic.Init(hWnd);

	//---------------------------------------------------------------

	//	2-4) ������ ������ ȭ�鿡 ���
	ShowWindow(hWnd, nShowCmd);		// ȭ�鿡 â ����
	UpdateWindow(hWnd);				// Ŭ���̾�Ʈ ���� ����

	// 3. �޽��� ���� ���� (+ ������ ���ν��� ����� -> WindowProc ���� ó��)
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	// �޽��� ����
		DispatchMessage(&msg);	// �޽��� ����
	}


	// 4. ���� �� GDI+ �˴ٿ� �� ���� �ڵ� ��ȯ
	// GDI+ �˴ٿ� ���� ���� �̹����� ��� �����ؾ� �Ѵ�.
	gLogic.Release();
	GdiplusShutdown(gdiToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hWnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);
	//-------------------------------------------------

	gLogic.Draw(graphics);

	//-------------------------------------------------
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONUP:
			gLogic.OnClick(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_PAINT:
			OnPaint(hWnd);
			break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}