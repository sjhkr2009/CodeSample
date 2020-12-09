#include <Windows.h>
#include "D2DFramework.h"

const wchar_t gClassName[] = L"MyWindowClass";

// ���� ���ӿ��� �����ӿ�ũ�� ��ӹ��� �� ������ �����ϴ� ������ ����Ѵ�.
class Tetris : public D2DFramework { };

// �����ӿ�ũ �׽�Ʈ�� ��ü ���� (�ʱ�ȭ�� ������ ���� �Ŀ�)
Tetris myFramework;

void ErrorBox(LPCWSTR errorText) { MessageBox(nullptr, errorText, L"Error", MB_OK); }
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	
	
	HWND hwnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
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

	RECT wr{ 0,0,1024,768 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Direct2D",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(wr.right - wr.left), (wr.bottom - wr.top),
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == nullptr)
	{
		ErrorBox(L"Failed to Create Window!");
		return 1;
	}

	// Direct 2D �����ӿ�ũ �ʱ�ȭ
	myFramework.Initialize(hwnd);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

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
			// �׸��� ����. ������ ���� OnPaint ��� ������ ȣ���Ͽ� �ϵ���� ������ ���� ��� �׸���.
			myFramework.Render();
		}
	}

	// ���α׷� ���� �� �޸� ����
	myFramework.Release();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_PAINT �޽����� ����Ʈ�� �Ѱܼ� �⺻ �׸��� ������ �����ϰ� �Ѵ�.

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
