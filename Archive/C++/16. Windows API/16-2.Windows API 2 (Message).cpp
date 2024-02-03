#include <Windows.h>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// 윈도우 API

// 1. 윈도우 클래스를 OS에 등록 (프로그램은 인스턴스!)
// 2. 클래스로 윈도우 생성
// 3. 윈도우 메시지 루프 만들기
// 4. 메시지 프로시저 (윈도우 프로시저)
//		* 윈도우는 Queue 방식으로 사용자의 입력(메시지)을 받아 처리한다.

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
		// 버퍼를 0으로 채워 초기화한다.

	wc.style = CS_HREDRAW | CS_VREDRAW;
		// Horizontal / Vertical Redraw : 가로/세로로 다시 그리는 것
	wc.lpszClassName = L"MyWindowClass";
		// 클래스 이름 지정
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 윈도우용 프로그램에서는 모던 C++ 스타일의 nullptr 대신 NULL 표기를 권장하고 있다.
		// 프로그램에서 사용될 마우스 커서 모양 지정
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		// 백그라운드 색상
	wc.lpfnWndProc = WindowProc;
		// 프로시저 생성 - 함수 포인터 사용
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		// OS에 등록하되, 실패 시 메시지를 띄우고 프로그램을 종료한다.
		MessageBox(nullptr, L"Failed To Register", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	
// 2. 클래스로 윈도우 생성
	hWnd = CreateWindowEx(
		0,					
		L"MyWindowClass",	// 클래스명
		L"SimpleWindow",	// 창 이름
		WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480,		// x, y, w, h
		NULL,				// 상위(부모) 클래스
		NULL,
		hInstance,
		NULL				// 추가로 넘겨줄 매개변수들
	);
	// * 워드(Word) : CPU가 한번에 처리할 수 있는 데이터의 크기에서 유래했으나, 32비트 때 고정되어 현재는 그냥 32비트를 가리킨다. DWORD는 더블 워드로 64비트.

	if (hWnd == NULL)
	{
		// 윈도우 생성에 실패하면, 메시지를 띄우고 프로그램을 종료한다.
		MessageBox(nullptr, L"Failed To Create", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// 생성한 창을 nShowCmd에서 받아온 양식에 맞게 출력하고, 윈도우를 업데이트해서 화면에 출력한다.
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);


// 3. 윈도우 메시지 처리
	MSG msg;

	// window message: 중요하므로 반드시 처리해야 하는 메시지. 메시지를 보낸 후 처리를 완료하고 응답이 올 때까지 기다린다.
	// thread message: 메시지를 보낸 후에 응답을 기다리지 않는다.
	//	GetMessage()에서 두 종류의 메시지를 모두 받으려면 NULL, 필터링 없이 모두 가져오려면 마찬가지로 최소/최대값 모두 0으로 입력하면 된다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	// 메시지가 있으면 가져와서 처리하는 작업을 무한 반복한다.

	return msg.wParam; // 윈도우 표준은 메시지에서 잘 처리되었음을 보내주는 것이지만, 그냥 0을 리턴하기도 한다.
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 매개변수로 들어오는 message는 int 값이므로, switch문으로 일괄 처리할 수 있다.

	// 운영체제가 메시지를 넘겨줄 때 메시지의 번호 값을 넘겨주고, 그 메시지에서 사용되는 변수 2개를 넘겨준다.
	// message는 메시지 번호, wParam, lParam은 해당 변수 2개를 의미한다. 변수는 입력에 따라 다르다. (예를 들어 마우스 입력에서 변수 2개는 x,y 좌표)

	switch (message)
	{
		case WM_CLOSE:				// 윈도우 닫기 입력이면
			DestroyWindow(hwnd);	// 매개변수로 넘어온 창 hwnd를 파괴한다. 창만 파괴하고 프로그램이 끝나지는 않으므로, 창이 파괴되었을 때 보내는 WM_DESTROY 메시지에 대한 처리를 해야 한다.
			break;

		case WM_DESTROY:			// 창이 파괴되었으면
			PostQuitMessage(0);		// 프로그램 종료 메시지를 보낸다.
			break;

		case WM_LBUTTONDOWN:						// 왼쪽 버튼 눌렀을 때
		{
			std::wstringstream ss;				// DWORD는 Low-Word(16bit)와 High-Word(16bit)로 구성된다. Low는 X값을, High는 Y값을 가지고 있다.
			ss << "X : " << LOWORD(lParam) << ", Y : " << HIWORD(lParam);
			MessageBox(hwnd, ss.str().c_str(), L"Button Down", MB_OK);
			break;
		}

		case WM_KEYDOWN:							// 시스템 키 (Ctrl + C 등 조합 키나 제조사 특수 키) 제외한 키가 입력되었을 때
		{
			std::wstringstream ss;				
			ss << "Virtual Key : " << wParam << ", lParam " << std::hex << lParam << std::endl;
			OutputDebugString(ss.str().c_str());	// 디버그 창에 내용을 찍어볼수도 있다.
			MessageBox(hwnd, ss.str().c_str(), L"Button Down", MB_OK);
			break;
		}

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	
	return 0;
}
