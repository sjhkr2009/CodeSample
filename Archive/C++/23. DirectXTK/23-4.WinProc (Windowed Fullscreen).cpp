#include "PCH.h"
#include "Game.h"

using namespace DirectX;

namespace
{
	std::unique_ptr<Game> gGame;
}

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// 함수를 컴파일할 때 C와 C++은 다른 형태로 만든다.
// C언어는 작성한 코드 그대로 컴파일되지만, C++은 다형성으로 인해 변수나 함수가 어디에 속하는지에 대한 정보가 필요하다.
// extern "C" 키워드는 C 스타일로 컴파일하라는 의미. 다형성 정보나 클래스 정보들이 불필요하다.
extern "C"
{
	// __declspec(ext-attr): 속성 선언(Declare Specification). 다음에 오는 선언자가 어떻게 동작할지 정보를 제공한다. extend-attribute에 키워드를 넣어 사용한다. (마이크로소프트 전용)
	// DLL Export: dll로 내보낸다. 그래픽카드 드라이버의 dll(보통 C형태로 작성됨)에 이 이름의 변수가 들어 있다는 의미.
	// 즉 그래픽카드 드라이버의 특정 옵션을 변경한다. (dll에 이 변수가 없으면 아무 동작도 하지 않음)
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;		// Nvidia 그래픽카드 드라이버에 들어 있는 옵티머스 기능이 이 프로그램에 항상 고성능의 GPU를 사용하도록 설정한다.
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;	// AMD 그래픽카드 드라이버에 들어 있는 Power X Press 기능이 이 프로그램에 항상 고성능의 GPU를 사용하도록 설정한다.

	// * Optimus / PowerXPress : GPU가 2개 이상일 경우 용도에 맞게 GPU를 전환하는 기능.
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	// 매개변수를 사용하지 않으면 컴파일러 설정에 따라 에러가 뜨므로, 사용하지 않음을 명시한다.
	// 내부적으로는 그냥 매개변수를 한 번 사용하는 척 하는 것. (param)으로 정의되어 있다.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr;

	// XMVerifyCPUSupport: CPU에서 SIMD를 포함한 DirectX 수학 연산을 위한 기능을 지원하는지 확인한다.
	if (!XMVerifyCPUSupport())
	{
		return 1;
	}

	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		return 1;
	}

	gGame = std::make_unique<Game>();

	{
		WNDCLASSEXW wcex{};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WinProc;
		wcex.hInstance = hInstance;
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszClassName = L"DirectXTKSimpleSampleWindowClass";
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

		if (!RegisterClassExW(&wcex))
		{
			return 1;
		}

		int w, h;
		gGame->GetDefaultSIze(w, h);

		RECT rect = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		HWND hwnd = CreateWindowExW(
			0,
			L"DirectXTKSimpleSampleWindowClass",
			L"DirectXTKSimpleSample",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);

		if (!hwnd)
		{
			return 1;
		}

		ShowWindow(hwnd, nCmdShow);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(gGame.get()));

		// AdjustWindowRect에서 창 전체 크기로 변환했던 rect에 다시 클라이언트 영역만 가져온다.
		GetClientRect(hwnd, &rect);
		gGame->Initialize(hwnd, rect.right - rect.left, rect.bottom - rect.top);
	}

	MSG msg{};
	while (WM_QUIT != msg.message) // ==, != 등을 = 으로 오타낼까봐 이런 스타일로 쓰는 경우도 있다. 리터럴이 좌측값으로 가면 대입 연산자로 오타 시 컴파일 에러를 내기 때문.
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gGame->Tick();
		}
	}

	gGame.reset();

	CoUninitialize();

	return static_cast<int>(msg.wParam);
}

// 메시지에 의해 콜백 방식으로 동작하는 윈도우 프로시저입니다.
// WM_CLOSE:	DestroyWindow() -> WM_DESTROY
// * 윈도우 관련
//	ㄴ s_in_sizeMove: 창 크기 변경
// WM_PAINT:			창 크기 변경중에도 게임 로직 실행
// WM_MOVE:				디스플레이 이동 시 OnWindowMoved() 호출
// WM_SIZE:				최소화 진입/해제 시 일시정지/해제. 최대화 시 OnWindowSizeChanged 호출
// WM_EXITSIZEMOVE:		창 크기 변경이 완료되면 OnWindowSizeChanged 호출
// WM_GETMINMAXINFO:	최소 크기 320x200 (임시)
// * 프로그램 활성화 관련
// WM_ACTIVATEAPP:		프로그램 활성화/비활성화 시 호출
// WM_POWERBROADCAST:	절전 모드 진입 / 사용자 입력에 의해 재개(Resume)될 경우 게임 일시정지 및 해제
// * 입출력 관련
// WM_MENUCHAR:			(Alt키를 통한)메뉴 호출 무시
// 마우스 입력:			마우스 이동, 휠 스크롤, 호버, 좌,우,중앙,측면 버튼에 대한 처리. 특수 키는 WM_INPUT으로 처리한다.
// 키보드 입력:			키 입력에 대한 처리. (시스템 키 포함)
//	ㄴ WM_SYSKEYDOWN:	Alt + Enter에 대해 전체 창 모드 동작을 별도로 입력한다.
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 윈도우 상태 추적을 위한 변수들. 이런 간단한 변수들은 정적 변수로 많이 사용한다.
	static bool s_in_sizeMove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;

	auto game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			if (s_in_sizeMove && game)
			{
				game->Tick();
			}
			else
			{
				PAINTSTRUCT ps;
				(void)BeginPaint(hwnd, &ps);	// 반환값이 있는 함수의 반환값을 명시적으로 사용하지 않을 경우, void로 변환한다.
				EndPaint(hwnd, &ps);
			}
			break;

		case WM_MOVE:
			if (game)
			{
				game->OnWindowMoved();
			}
			break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				if (!s_minimized)
				{
					s_minimized = true;
					if (!s_in_suspend && game)
					{
						game->OnSuspending();
					}
					s_in_suspend = true;
				}
			}
			else if (s_minimized)
			{
				s_minimized = false;
				if (s_in_suspend && game)
				{
					game->OnResuming();
				}
				s_in_suspend = false;
			}
			else if (!s_in_sizeMove && game)
			{
				game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
			}
			break;

		case WM_ENTERSIZEMOVE:
			s_in_sizeMove = true;
			break;

		case WM_EXITSIZEMOVE:
			s_in_sizeMove = false;
			if (game)
			{
				RECT rect;
				GetClientRect(hwnd, &rect);

				game->OnWindowSizeChanged(rect.right - rect.left, rect.bottom - rect.top);
			}
			break;

		case WM_GETMINMAXINFO:
			if (lParam)
			{
				auto info = reinterpret_cast<MINMAXINFO*>(lParam);
				info->ptMinTrackSize.x = 320;
				info->ptMinTrackSize.y = 200;
			}
			break;

		// 프로그램의 활성화 여부가 변경되었을 때 호출된다.
		case WM_ACTIVATEAPP:
			if (game)
			{
				if (wParam)
				{
					game->OnActivated();
				}
				else
				{
					game->OnDeactivated();
				}
			}
			break;

		// 윈도우 전원 모드 변경 (절전 모드 on/off)
		case WM_POWERBROADCAST:
			switch (wParam)
			{
				// 해당 처리를 하지 않으면, 최대 절전 모드 시 그래픽카드 정보
				case PBT_APMQUERYSUSPEND:	// 절전 모드 진입
					if (!s_in_suspend && game)
					{
						game->OnSuspending();
					}
					s_in_suspend = true;
					return TRUE;	// 절전 모드 동작을 여기서 실행했으니, 메시지를 다른 곳으로 넘기지 않고 정상 처리됨(TRUE)을 반환한다.

				case PBT_APMRESUMESUSPEND:	// 사용자 입력에 의한 절전 모드 해제 (평시 상태로 돌아옴)
					if (!s_minimized)
					{
						if (s_in_suspend && game)
						{
							game->OnResuming();
						}
						s_in_suspend = false;
					}
					return TRUE;
			}
			break;

		// 마우스 입력에 대한 처리
		case WM_INPUT:		// 메시지 루프에서 번역되지 않은 값(Raw Data). 버튼이 많은 마우스나 특수 동작 입력 등 번역에 실패하면 여기로 넘어온다.
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER:	// 마우스가 일정 시간 멈춰 있을 때. 특정 버튼 위에 마우스를 대고 있으면 설명이 뜨는 등의 동작에 사용된다.
		case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:	// 마우스 좌측 엄지손가락 쪽에 달린 버튼
			Mouse::ProcessMessage(msg, wParam, lParam);
			break;

		// 키보드 입력에 대한 처리
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;

		// 시스템 키 입력에 대한 추가 처리
		case WM_SYSKEYDOWN:
			// lParam을 0x60000000으로 마스킹해서 0x20000000인지 확인한다. 즉 29번 비트는 켜져 있고, 30번 비트는 꺼져 있어야 한다.
			// (MSDN 참조) 29번 비트는 Alt 키가 눌려 있으면 켜진다. 30번 비트는 지금 누른 키가 이전에도 눌려져 있었다면 켜진다.
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)	// Alt + Enter 입력 시 처리
			{
				if (!s_fullscreen)
				{
					// 기본 전체화면 모드는 사용하지 않고 전체 창 모드로 대체했으니, 그에 맞는 스타일로 변경한다.
					// 전체 화면 시 타이틀 바와 외곽선 등 윈도우 스타일을 모두 제거하고, 이 창을 TOPMOST 스타일(Alt+Tab 등으로 작업이 전환되지 않는 한 다른 앱 앞에 위치하게)로 지정한다.
					SetWindowLongPtr(hwnd, GWL_STYLE, 0);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					// 윈도우 세팅. 위치나 크기, Z순서는 무시하고 프레임 변경만 적용한다.
					// HWND_TOP: 운영체제가 관리하는 윈도우 목록의 최상단. 이 다음에 온다는 건 입력한 윈도우 핸들을 다른 창들의 앞에 놓는다는 의미.
					// 플래그: 위치 변경 안 함(= x,y 무시), 크기 변경 안 함(= cx, cy 무시), Z오더(창 출력 순서) 변경 안 함, 윈도우 테두리 변경됨을 OS에 알림
					SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

					// 클라이언트 영역을 최대화해서 보여준다.
					ShowWindow(hwnd, SW_SHOWMAXIMIZED);
				}
				else
				{
					// 전체 창 모드에서 변경한 스타일을 원래대로 되돌린다.
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

					int w{ 800 }, h{ 600 };
					if (game)
					{
						game->GetDefaultSIze(w, h);
					}
					ShowWindow(hwnd, SW_SHOWNORMAL);

					SetWindowPos(hwnd, HWND_TOP, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				s_fullscreen = !s_fullscreen;
			}

			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;

		// Alt를 통한 메뉴 접근은 무시하도록 한다. (메뉴를 닫고 메시지 정상 처리로 반환)
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ExitGame() noexcept
{
	PostQuitMessage(0);
}