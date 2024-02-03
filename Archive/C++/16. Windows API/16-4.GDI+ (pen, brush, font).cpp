#include <Windows.h>
#include <sstream>

// GDI+

// 기존 GDI는 Win32에 포함되어 있지만, GDI+는 포함되어 있지 않다.
// 별개의 헤더를 include 하고, 헤더에 선언된 내용이 정의되어 있는곳을 찾아갈 수 있게 해야 한다. Gdiplus.lib에는 dll 파일의 주소가 들어 있다.
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

// GDI+는 C++ 기반으로, Gdiplus 네임스페이스에 기능들이 들어 있다.
using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void OnPaint(HWND hwnd);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// GDI+ 초기화----------------------
	GdiplusStartupInput gdiStart;
	ULONG_PTR token;
	
	// Input으로 기본값을 넘긴 후, 토큰을 받아온다. output은 받지 않으므로 nullptr을 넣는다.
	// 이 함수 끝부분에서 토큰을 통해 Gdiplus 기능을 셧다운시킬 것이다.
	GdiplusStartup(&token, &gdiStart, nullptr);

	// GdiplusStartupInput ( 버전, 프로시저, 백그라운드 실행 차단 여부, 외부 코덱 차단 여부 ) - 기본값이 들어 있으니 선언만 하면 됨.
	// 버전은 더 이상 업데이트하지 않으니 항상 1, 프로시저는 콜백 함수의 포인터가 들어간다.
	// 백그라운드에서 동작하는 내용을 막으면 교착 상태를 방지할 수 있지만, 실행 속도는 느려진다.
	// 버전 1에서는 외부 코덱을 지원하지 않으므로 무시된다. 디폴트값은 false.


	//----------------------------------
	
	HWND hWnd;
	WNDCLASSEX wc;

	// 1. 윈도우 클래스 -> OS 등록
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"MyWindowClass";
	
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// 2. 클래스로 윈도우 생성
	hWnd = CreateWindowEx(
		0,
		L"MyWindowClass",	
		L"SimpleWindow",
		WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480,			// 타이틀 바와 창 테두리를 포함한 전체 크기를 나타낸다. 프로그램의 해상도와는 약간 차이가 있다.
		NULL,				
		NULL,
		hInstance,
		NULL
	);

	if (hWnd == NULL)
	{
		MessageBox(nullptr, L"Failed To Create", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);


	// 3. 윈도우 메시지 처리
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	// GDI+를 종료한다.
	GdiplusShutdown(token);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_CLOSE:				
			DestroyWindow(hwnd);	
			break;

		case WM_DESTROY:		
			PostQuitMessage(0);	
			break;

		case WM_PAINT:
		{
			OnPaint(hwnd);
			
			break;
		}

		default:
			return DefWindowProc(hwnd, message, wParam, lParam); // 윈도우 기본 규격에 대한 처리를 해 준다. (배경색을 칠하는 처리 등)
	}

	return 0;
}

void OnPaint(HWND hwnd)
{
	// 기본 구조는 GDI와 동일. DC 핸들과 PaintStruct 구조체가 필요하며, Begin/End 사이에 그릴 내용을 입력한다.
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	//-------------------------------------------------

	Graphics graphics(hdc);

	Pen pen(Color(255, 0, 0, 255));					// GDI+ 에서는 a,r,g,b를 입력하여 해당 색상의 펜을 만들 수 있고, 함수가 끝나면 메모리가 해제되므로 Release가 불필요하다.
	graphics.DrawRectangle(&pen, 0, 0, 100, 100);

	SolidBrush sBrush(Color(255, 255, 0, 255));		// 단색 브러쉬는 SolidBrush를 사용한다.
	FontFamily fontFamily(L"맑은 고딕");			// FontFamily는 글꼴을 의미한다. 글꼴 내에 가늘게, 보통, 굵게 등 여러 바리에이션을 포함한 통합 글꼴을 FontFamily라고 한다.
													// 폰트는 내부적으로 em(엠) 사이즈라는 변수를 가진다. 과거 활자판의 길이를 나타내던 개념으로, 1em은 약 16픽셀(12pt) 정도이다.
	Font font(&fontFamily, 24, FontStyle::FontStyleRegular, UnitPixel);	// 폰트를 구체적으로 설정한다. 변수로는 (폰트 패밀리의 주소, 크기, 스타일(FontStyle), 크기 단위)
													// UnitPixel은 픽셀 단위를 의미하므로, 두 번째 변수 24가 24픽셀이 된다.

	PointF origin(0.f, 110.f);						// 좌표 지정. 왼쪽 상단에서 (x, y) 만큼 떨어진 위치에서 쓰기 시작한다.
	// 글자 출력 때는 (입력할 내용, 길이, 폰트의 주소, 좌표, 브러쉬 주소) 를 넘겨준다. 길이는 안전을 위해 문자열 길이를 받는 것인데, -1를 입력하면 입력한 내용만큼 자동으로 지정된다.
	graphics.DrawString(L"폰트 테스트", -1, &font, origin, &sBrush);

	//-------------------------------------------------
	EndPaint(hwnd, &ps);
}