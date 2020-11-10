#include <Windows.h>
#include "D2DFramework.h"

const wchar_t gClassName[] = L"MyWindowClass";

// 만들 게임에서 프레임워크를 상속받은 후 본문을 구성하는 식으로 사용한다.
class Tetris : public D2DFramework { };

// 프레임워크 테스트용 객체 선언 (초기화는 윈도우 생성 후에)
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

	// Direct 2D 프레임워크 초기화
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
			// 그리기 동작. 기존에 쓰던 OnPaint 대신 렌더를 호출하여 하드웨어 가속을 통해 계속 그린다.
			myFramework.Render();
		}
	}

	// 프로그램 종료 시 메모리 해제
	myFramework.Release();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_PAINT 메시지는 디폴트로 넘겨서 기본 그리기 동작을 수행하게 한다.

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
