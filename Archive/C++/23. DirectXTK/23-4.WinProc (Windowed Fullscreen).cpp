#include "PCH.h"
#include "Game.h"

using namespace DirectX;

namespace
{
	std::unique_ptr<Game> gGame;
}

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// �Լ��� �������� �� C�� C++�� �ٸ� ���·� �����.
// C���� �ۼ��� �ڵ� �״�� �����ϵ�����, C++�� ���������� ���� ������ �Լ��� ��� ���ϴ����� ���� ������ �ʿ��ϴ�.
// extern "C" Ű����� C ��Ÿ�Ϸ� �������϶�� �ǹ�. ������ ������ Ŭ���� �������� ���ʿ��ϴ�.
extern "C"
{
	// __declspec(ext-attr): �Ӽ� ����(Declare Specification). ������ ���� �����ڰ� ��� �������� ������ �����Ѵ�. extend-attribute�� Ű���带 �־� ����Ѵ�. (����ũ�μ���Ʈ ����)
	// DLL Export: dll�� ��������. �׷���ī�� ����̹��� dll(���� C���·� �ۼ���)�� �� �̸��� ������ ��� �ִٴ� �ǹ�.
	// �� �׷���ī�� ����̹��� Ư�� �ɼ��� �����Ѵ�. (dll�� �� ������ ������ �ƹ� ���۵� ���� ����)
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;		// Nvidia �׷���ī�� ����̹��� ��� �ִ� ��Ƽ�ӽ� ����� �� ���α׷��� �׻� ������ GPU�� ����ϵ��� �����Ѵ�.
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;	// AMD �׷���ī�� ����̹��� ��� �ִ� Power X Press ����� �� ���α׷��� �׻� ������ GPU�� ����ϵ��� �����Ѵ�.

	// * Optimus / PowerXPress : GPU�� 2�� �̻��� ��� �뵵�� �°� GPU�� ��ȯ�ϴ� ���.
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	// �Ű������� ������� ������ �����Ϸ� ������ ���� ������ �߹Ƿ�, ������� ������ ����Ѵ�.
	// ���������δ� �׳� �Ű������� �� �� ����ϴ� ô �ϴ� ��. (param)���� ���ǵǾ� �ִ�.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HRESULT hr;

	// XMVerifyCPUSupport: CPU���� SIMD�� ������ DirectX ���� ������ ���� ����� �����ϴ��� Ȯ���Ѵ�.
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

		// AdjustWindowRect���� â ��ü ũ��� ��ȯ�ߴ� rect�� �ٽ� Ŭ���̾�Ʈ ������ �����´�.
		GetClientRect(hwnd, &rect);
		gGame->Initialize(hwnd, rect.right - rect.left, rect.bottom - rect.top);
	}

	MSG msg{};
	while (WM_QUIT != msg.message) // ==, != ���� = ���� ��Ÿ����� �̷� ��Ÿ�Ϸ� ���� ��쵵 �ִ�. ���ͷ��� ���������� ���� ���� �����ڷ� ��Ÿ �� ������ ������ ���� ����.
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

// �޽����� ���� �ݹ� ������� �����ϴ� ������ ���ν����Դϴ�.
// WM_CLOSE:	DestroyWindow() -> WM_DESTROY
// * ������ ����
//	�� s_in_sizeMove: â ũ�� ����
// WM_PAINT:			â ũ�� �����߿��� ���� ���� ����
// WM_MOVE:				���÷��� �̵� �� OnWindowMoved() ȣ��
// WM_SIZE:				�ּ�ȭ ����/���� �� �Ͻ�����/����. �ִ�ȭ �� OnWindowSizeChanged ȣ��
// WM_EXITSIZEMOVE:		â ũ�� ������ �Ϸ�Ǹ� OnWindowSizeChanged ȣ��
// WM_GETMINMAXINFO:	�ּ� ũ�� 320x200 (�ӽ�)
// * ���α׷� Ȱ��ȭ ����
// WM_ACTIVATEAPP:		���α׷� Ȱ��ȭ/��Ȱ��ȭ �� ȣ��
// WM_POWERBROADCAST:	���� ��� ���� / ����� �Է¿� ���� �簳(Resume)�� ��� ���� �Ͻ����� �� ����
// * ����� ����
// WM_MENUCHAR:			(AltŰ�� ����)�޴� ȣ�� ����
// ���콺 �Է�:			���콺 �̵�, �� ��ũ��, ȣ��, ��,��,�߾�,���� ��ư�� ���� ó��. Ư�� Ű�� WM_INPUT���� ó���Ѵ�.
// Ű���� �Է�:			Ű �Է¿� ���� ó��. (�ý��� Ű ����)
//	�� WM_SYSKEYDOWN:	Alt + Enter�� ���� ��ü â ��� ������ ������ �Է��Ѵ�.
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ������ ���� ������ ���� ������. �̷� ������ �������� ���� ������ ���� ����Ѵ�.
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
				(void)BeginPaint(hwnd, &ps);	// ��ȯ���� �ִ� �Լ��� ��ȯ���� ��������� ������� ���� ���, void�� ��ȯ�Ѵ�.
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

		// ���α׷��� Ȱ��ȭ ���ΰ� ����Ǿ��� �� ȣ��ȴ�.
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

		// ������ ���� ��� ���� (���� ��� on/off)
		case WM_POWERBROADCAST:
			switch (wParam)
			{
				// �ش� ó���� ���� ������, �ִ� ���� ��� �� �׷���ī�� ����
				case PBT_APMQUERYSUSPEND:	// ���� ��� ����
					if (!s_in_suspend && game)
					{
						game->OnSuspending();
					}
					s_in_suspend = true;
					return TRUE;	// ���� ��� ������ ���⼭ ����������, �޽����� �ٸ� ������ �ѱ��� �ʰ� ���� ó����(TRUE)�� ��ȯ�Ѵ�.

				case PBT_APMRESUMESUSPEND:	// ����� �Է¿� ���� ���� ��� ���� (��� ���·� ���ƿ�)
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

		// ���콺 �Է¿� ���� ó��
		case WM_INPUT:		// �޽��� �������� �������� ���� ��(Raw Data). ��ư�� ���� ���콺�� Ư�� ���� �Է� �� ������ �����ϸ� ����� �Ѿ�´�.
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER:	// ���콺�� ���� �ð� ���� ���� ��. Ư�� ��ư ���� ���콺�� ��� ������ ������ �ߴ� ���� ���ۿ� ���ȴ�.
		case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:	// ���콺 ���� �����հ��� �ʿ� �޸� ��ư
			Mouse::ProcessMessage(msg, wParam, lParam);
			break;

		// Ű���� �Է¿� ���� ó��
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;

		// �ý��� Ű �Է¿� ���� �߰� ó��
		case WM_SYSKEYDOWN:
			// lParam�� 0x60000000���� ����ŷ�ؼ� 0x20000000���� Ȯ���Ѵ�. �� 29�� ��Ʈ�� ���� �ְ�, 30�� ��Ʈ�� ���� �־�� �Ѵ�.
			// (MSDN ����) 29�� ��Ʈ�� Alt Ű�� ���� ������ ������. 30�� ��Ʈ�� ���� ���� Ű�� �������� ������ �־��ٸ� ������.
			if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)	// Alt + Enter �Է� �� ó��
			{
				if (!s_fullscreen)
				{
					// �⺻ ��üȭ�� ���� ������� �ʰ� ��ü â ���� ��ü������, �׿� �´� ��Ÿ�Ϸ� �����Ѵ�.
					// ��ü ȭ�� �� Ÿ��Ʋ �ٿ� �ܰ��� �� ������ ��Ÿ���� ��� �����ϰ�, �� â�� TOPMOST ��Ÿ��(Alt+Tab ������ �۾��� ��ȯ���� �ʴ� �� �ٸ� �� �տ� ��ġ�ϰ�)�� �����Ѵ�.
					SetWindowLongPtr(hwnd, GWL_STYLE, 0);
					SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					// ������ ����. ��ġ�� ũ��, Z������ �����ϰ� ������ ���游 �����Ѵ�.
					// HWND_TOP: �ü���� �����ϴ� ������ ����� �ֻ��. �� ������ �´ٴ� �� �Է��� ������ �ڵ��� �ٸ� â���� �տ� ���´ٴ� �ǹ�.
					// �÷���: ��ġ ���� �� ��(= x,y ����), ũ�� ���� �� ��(= cx, cy ����), Z����(â ��� ����) ���� �� ��, ������ �׵θ� ������� OS�� �˸�
					SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

					// Ŭ���̾�Ʈ ������ �ִ�ȭ�ؼ� �����ش�.
					ShowWindow(hwnd, SW_SHOWMAXIMIZED);
				}
				else
				{
					// ��ü â ��忡�� ������ ��Ÿ���� ������� �ǵ�����.
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

		// Alt�� ���� �޴� ������ �����ϵ��� �Ѵ�. (�޴��� �ݰ� �޽��� ���� ó���� ��ȯ)
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ExitGame() noexcept
{
	PostQuitMessage(0);
}