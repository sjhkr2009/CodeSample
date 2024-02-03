#include <Windows.h>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// GDI (Device Context)
// 참고) 빈 솔루션의 엔트리 포인트를 윈도우용으로 전환하려면, [프로젝트] - [속성] - [링커] - [시스템] 탭에서 '하위 시스템'을 콘솔에서 창으로 변경할 것.

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
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

		case WM_LBUTTONDOWN:		// 왼쪽 버튼 눌렀을 때
		{
			HDC hdc = NULL;			// Handle of Device Context : 디바이스 컨텍스트 핸들
			hdc = GetDC(hwnd);		// hWnd(Handle of Window)를 넘겨준다.
			
			// 출력할 내용을 입력한다. 여기서는 100x100의 사각형을 그린다. 색상이나 테두리 선 등이 기본적으로 제공된다.
			Rectangle(hdc, 0, 0, 100, 100);

			ReleaseDC(hwnd, hdc);	// 출력할 내용 입력이 끝나면 ReleaseDC를 통해 해제시킨다.

			// DC는 GetDC에서 가져와서 화면을 1회 그리고 ReleaseDC에서 해제된다.
			// 화면을 최소화 혹은 창 크기를 변경하면 그려진 사각형이 지워지는 것을 확인할 수 있다.
			break;
		}

		// 윈도우 배경: 윈도우 창에 기본적으로 적용된 창의 테두리, 상단 타이틀 바, 최소/최대화 및 닫기 버튼 등
		/*
		case WM_ERASEBKGND:			// 윈도우 배경이 지워질 때 = WM_PAINT에서 자동으로 호출된다. 기본적으로 창을 배경색으로 칠해주는 역할을 한다.
			break;					// 직접 이렇게 케이스를 추가하면 기본 기능이 오버라이드되므로 호출되지 않는다. 즉 창의 배경이 자동으로 칠해지지 않는다.
		*/
		
		// GDI의 핵심은 WM_PAINT를 통해 그릴 내용을 출력하는 것.
		case WM_PAINT:				// 윈도우 배경이 칠해질 때 = 창을 열거나, 창 크기를 바꾸거나, 최소화된 창을 다시 활성화할 때, 그려야 할 내용이 변할 때 호출
		{
			PAINTSTRUCT ps;			// 어디를 그릴지, 배경을 지울지 말지 등의 정보를 담고 있다. 선언만 해서 넘겨주면 윈도우에서 자동으로 세팅해준다.

			// 프로그래머는 BeginPaint와 EndPaint 사이에 그릴 내용을 입력한다.
			HDC hdc = BeginPaint(hwnd, &ps);

		// 1) 브러쉬

			// 브러쉬는 그린 내용을 칠할 때 사용된다. 기본적으로는 흰 배경에 아무 무늬가 없지만, 브러쉬를 지정하면 해당 내용으로 칠해진다.
			// 첫 번째 매개변수는 브러쉬의 종류를 입력한다. (빗금 형태인 해치 브러쉬는 HS_xx 형태. HS는 해치 스타일.)
			// 두 번째 변수의 COLORREF는 4바이트의 구조체(DWORD)로, 색상을 나타내는 숫자 값이 들어간다. 각 바이트는, a, r, g, b값을 의미한다. 예를 들어, 0xFFFF0000 = 빨강색.
			// 이는 RGB(r,g,b)에서 0~255의 값을 입력하는 것으로 대신할 수 있다,
			HBRUSH hatchBrush = CreateHatchBrush(HS_CROSS, RGB(255, 0, 0));

			// DC 핸들에 브러쉬를 적용시키고 사각형을 그리면, 흰 배경 대신 지정한 브러쉬로 칠해지는 것을 확인할 수 있다.
			// SelectObject는 변경하기 전의 브러쉬를 반환한다.
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hatchBrush);
			Rectangle(hdc, 0, 0, 100, 100);

			SelectObject(hdc, oldBrush);
			Rectangle(hdc, 100, 100, 200, 200);


		// 2) 테두리 선

			// 선의 스타일, 굵기, 색상을 지정한다.
			HPEN bluePen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));

			// 적용은 공통적으로 SelectObject를 사용한다. 역시 기존의 선을 반환한다.
			SelectObject(hdc, bluePen);

			Rectangle(hdc, 200, 200, 300, 300);

			
		// 우리가 직접 만든 객체는 자동 해제가 안 되므로, 사용이 끝나면 직접 지워준다. (기본 브러쉬는 지울 필요 없음)
			DeleteObject(hatchBrush);
			DeleteObject(bluePen);

			EndPaint(hwnd, &ps);
			
			break;
		}

		default:
			return DefWindowProc(hwnd, message, wParam, lParam); // 윈도우 기본 규격에 대한 처리를 해 준다. (배경색을 칠하는 처리 등)
	}

	return 0;
}
