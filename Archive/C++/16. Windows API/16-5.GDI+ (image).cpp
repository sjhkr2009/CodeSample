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

	// 기존에는 창 전체 크기를 640x480으로 생성했지만, 게임에서 해상도가 640x480이 되려면 클라이언트 영역(테두리와 상단 바 등을 제외한 내용 표기 영역)이 640x480이어야 한다.
	// 클라이언트 영역을 지정하여 윈도우를 만들어보자.

	// 1) 클라이언트 영역을 Rect로 생성한다.
	RECT wr = { 0, 0, 640, 480 };

	// 2) AdjustWindowRect로 운영체제에 Rect를 넘기면, 창 크기를 조절하여 wr을 바꿔준다.
	//		ㄴ매개변수로는 (Rect의 주소, dwStyle, 메뉴 바의 유무)를 입력한다. dwStyle은 아래 CreateWindowEx와 동일하게 입력한다.
	AdjustWindowRect((LPRECT)&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(
		0,
		L"MyWindowClass",	
		L"SimpleWindow",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(wr.right - wr.left), (wr.bottom - wr.top),
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

	Image image(L"BabyMurloc.png");		// GDI+ 에서는 Image로 이미지 주소를 넣으면 출력할 준비가 끝난다.
	graphics.DrawImage(&image, 0, 0);	// 이미지의 주소와 좌표값을 넘겨 이미지를 그린다. (여기서는 x,y 좌표를 하드코딩하기로 한다)
	// 실제로 코딩할 때는 이미지 로딩은 최초에 1번만 실행하고, OnPaint에서는 DrawImage만 실행한다.

	graphics.DrawImage(&image, 0, 0, 640, 480); // 시작 좌표만 주면 원본 파일 이미지 크기대로 그리는데, 가로 및 세로 크기도 지정할 수 있다.
	graphics.DrawImage(&image, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom); // 화면 크기에 대응하고 싶으면 위에서 선언한 PAINTSTRUCT를 이용한다. ps.rcPaint에 그릴 영역의 크기 정보가 들어 있다.

	// 아래와 같이 그릴 영역의 넓이와 높이를 구할 수 있다.
	int width{ ps.rcPaint.right - ps.rcPaint.left };
	int height{ ps.rcPaint.bottom - ps.rcPaint.top };

	// 이미지의 원본 크기로 지정하려면, image.Get~() 함수로 이미지 크기를 가져올 수 있다.
	// 기본적으로 크기를 입력하지 않아도 원본 이미지 크기로 그리지만, 디스플레이 옵션에 따라 다르게 보일 수 있다.
	// 따라서 원본 크기로 그리더라도 이미지 크기를 직접 지정하는 아래쪽 코드가 더 안전하다.
	graphics.DrawImage(&image, 0, 10);
	graphics.DrawImage(&image, 250, 10, image.GetWidth(), image.GetHeight());

	//-------------------------------------------------
	EndPaint(hwnd, &ps);
}