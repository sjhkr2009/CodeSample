// GameLogic.h을 통해 Windows.h 및 Gdiplus를 불러오므로, 별도의 include는 불필요하다.
#include "GameLogic.h"

#pragma comment ( lib, "Gdiplus.lib" )

const wchar_t gClassName[]{ L"MyWindowClass" };

// 솔리테어 게임 만들기 (= 같은그림 찾기)

// 테스트 코드
// ---------------------------------

Solitaire::GameLogic gLogic;

// ---------------------------------

// 참고
//
/*
	카드와 같은 시스템의 개별적인 요소들을 먼저 만들고, 이를 포괄하는 시스템을 만드는 방식을 '하향식 설계'(Bottom-Up Design)라 한다.
	이와 반대로 게임 매니저와 같이 큰 시스템을 먼저 만들고 개별적 요소를 만드는 방식을 '상향식 설계'(Top-Down Design)라 한다.
	과거엔 임시 코드와 개별적 테스트 필요 없이 전체 코드를 모아 컴파일할 수 있는 후자를 선호했지만, 최근엔 게임의 볼륨이 커져 개별적인 테스트가 가능한 전자를 선호한다.

	하향식 설계에서는 개별적 요소들이 잘 동작하는지 확실한 테스트가 필요하다. 이를 Unit Test라고 한다.
	과거 게임들이나, 게임 볼륨이 작고 (주로 팀장급에서) 설계가 잘 짜여져 있다면 상향식 설계로 더 빠른 개발을 추구하기도 한다.

	1인 개발이나 인디 개발은 즉흥적 테스트가 가능한 하향식 설계로 진행도니다.

*/ 



// 윈도우 프로시저 선언
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

	// 1. 윈도우 클래스 생성 및 등록
	//	1-1) 윈도우 핸들 및 윈도우 클래스를 위한 클래스EX 선언
	HWND hWnd;
	WNDCLASSEX wc;

	//	1-2) 메모리를 특정한 값(=0)으로 초기화
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//	1-3) WNDCLASSEX 구조체의 기본값들을 채워준다
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//	1-4) 초기화가 끝난 클래스를 등록 (+ 실패 시 예외처리)
	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register!", L"Error", MB_OK);
		return 0;
	}

	// 2. 윈도우 생성
	//	2-1) 윈도우 크기 지정
	RECT wr = { 0, 0, 1024, 768 };						// Client Rect. 우리가 의도하는 클라이언트 크기.
	AdjustWindowRect(&wr, (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION), FALSE);	// 클라이언트 크기에 원하는 스타일을 적용했을 때 실제 창 크기가 어떻게 될 지 계산하여 wr에 반영해준다.
	// WS_OVERLAPPEDWINDOW는 위에 다른 창이 오거나, 시스템 메뉴, 창 크기조절이 가능한 외곽선, 최대화, 최소화 등의 스타일이 포함되어 있다.
	// 우리는 창 크기 조절을 해버리면 배경 크기가 달라지므로, 오버랩과 시스템 메뉴, 상단 타이틀ㅌ바만 수동으로 적용한다.

	//	2-2) 윈도우 생성
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

	//	2-3) 예외 처리
	if (hWnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return 0;
	}

	//---------------------------------------------------------------
	
	gLogic.Init(hWnd);

	//---------------------------------------------------------------

	//	2-4) 생성된 윈도우 화면에 출력
	ShowWindow(hWnd, nShowCmd);		// 화면에 창 띄우고
	UpdateWindow(hWnd);				// 클라이언트 영역 갱신

	// 3. 메시지 루프 생성 (+ 윈도우 프로시저 만들기 -> WindowProc 에서 처리)
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	// 메시지 번역
		DispatchMessage(&msg);	// 메시지 전달
	}


	// 4. 종료 시 GDI+ 셧다운 및 에러 코드 반환
	// GDI+ 셧다운 전에 관련 이미지는 모두 해제해야 한다.
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