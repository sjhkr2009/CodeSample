#include <Windows.h>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// ������ API

// 1. ������ Ŭ������ OS�� ��� (���α׷��� �ν��Ͻ�!)
// 2. Ŭ������ ������ ����
// 3. ������ �޽��� ���� �����
// 4. �޽��� ���ν��� (������ ���ν���)
//		* ������� Queue ������� ������� �Է�(�޽���)�� �޾� ó���Ѵ�.

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	HWND hWnd;
	WNDCLASSEX wc;

// 1. ������ Ŭ���� -> OS ���
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
		// ���۸� 0���� ä�� �ʱ�ȭ�Ѵ�.

	wc.style = CS_HREDRAW | CS_VREDRAW;
		// Horizontal / Vertical Redraw : ����/���η� �ٽ� �׸��� ��
	wc.lpszClassName = L"MyWindowClass";
		// Ŭ���� �̸� ����
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // ������� ���α׷������� ��� C++ ��Ÿ���� nullptr ��� NULL ǥ�⸦ �����ϰ� �ִ�.
		// ���α׷����� ���� ���콺 Ŀ�� ��� ����
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		// ��׶��� ����
	wc.lpfnWndProc = WindowProc;
		// ���ν��� ���� - �Լ� ������ ���
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		// OS�� ����ϵ�, ���� �� �޽����� ���� ���α׷��� �����Ѵ�.
		MessageBox(nullptr, L"Failed To Register", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	
// 2. Ŭ������ ������ ����
	hWnd = CreateWindowEx(
		0,					
		L"MyWindowClass",	// Ŭ������
		L"SimpleWindow",	// â �̸�
		WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480,		// x, y, w, h
		NULL,				// ����(�θ�) Ŭ����
		NULL,
		hInstance,
		NULL				// �߰��� �Ѱ��� �Ű�������
	);
	// * ����(Word) : CPU�� �ѹ��� ó���� �� �ִ� �������� ũ�⿡�� ����������, 32��Ʈ �� �����Ǿ� ����� �׳� 32��Ʈ�� ����Ų��. DWORD�� ���� ����� 64��Ʈ.

	if (hWnd == NULL)
	{
		// ������ ������ �����ϸ�, �޽����� ���� ���α׷��� �����Ѵ�.
		MessageBox(nullptr, L"Failed To Create", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	// ������ â�� nShowCmd���� �޾ƿ� ��Ŀ� �°� ����ϰ�, �����츦 ������Ʈ�ؼ� ȭ�鿡 ����Ѵ�.
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);


// 3. ������ �޽��� ó��
	MSG msg;

	// window message: �߿��ϹǷ� �ݵ�� ó���ؾ� �ϴ� �޽���. �޽����� ���� �� ó���� �Ϸ��ϰ� ������ �� ������ ��ٸ���.
	// thread message: �޽����� ���� �Ŀ� ������ ��ٸ��� �ʴ´�.
	//	GetMessage()���� �� ������ �޽����� ��� �������� NULL, ���͸� ���� ��� ���������� ���������� �ּ�/�ִ밪 ��� 0���� �Է��ϸ� �ȴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	// �޽����� ������ �����ͼ� ó���ϴ� �۾��� ���� �ݺ��Ѵ�.

	return msg.wParam; // ������ ǥ���� �޽������� �� ó���Ǿ����� �����ִ� ��������, �׳� 0�� �����ϱ⵵ �Ѵ�.
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// �Ű������� ������ message�� int ���̹Ƿ�, switch������ �ϰ� ó���� �� �ִ�.

	// �ü���� �޽����� �Ѱ��� �� �޽����� ��ȣ ���� �Ѱ��ְ�, �� �޽������� ���Ǵ� ���� 2���� �Ѱ��ش�.
	// message�� �޽��� ��ȣ, wParam, lParam�� �ش� ���� 2���� �ǹ��Ѵ�. ������ �Է¿� ���� �ٸ���. (���� ��� ���콺 �Է¿��� ���� 2���� x,y ��ǥ)

	switch (message)
	{
		case WM_CLOSE:				// ������ �ݱ� �Է��̸�
			DestroyWindow(hwnd);	// �Ű������� �Ѿ�� â hwnd�� �ı��Ѵ�. â�� �ı��ϰ� ���α׷��� �������� �����Ƿ�, â�� �ı��Ǿ��� �� ������ WM_DESTROY �޽����� ���� ó���� �ؾ� �Ѵ�.
			break;

		case WM_DESTROY:			// â�� �ı��Ǿ�����
			PostQuitMessage(0);		// ���α׷� ���� �޽����� ������.
			break;

		case WM_LBUTTONDOWN:						// ���� ��ư ������ ��
		{
			std::wstringstream ss;				// DWORD�� Low-Word(16bit)�� High-Word(16bit)�� �����ȴ�. Low�� X����, High�� Y���� ������ �ִ�.
			ss << "X : " << LOWORD(lParam) << ", Y : " << HIWORD(lParam);
			MessageBox(hwnd, ss.str().c_str(), L"Button Down", MB_OK);
			break;
		}

		case WM_KEYDOWN:							// �ý��� Ű (Ctrl + C �� ���� Ű�� ������ Ư�� Ű) ������ Ű�� �ԷµǾ��� ��
		{
			std::wstringstream ss;				
			ss << "Virtual Key : " << wParam << ", lParam " << std::hex << lParam << std::endl;
			OutputDebugString(ss.str().c_str());	// ����� â�� ������ ������ �ִ�.
			MessageBox(hwnd, ss.str().c_str(), L"Button Down", MB_OK);
			break;
		}

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	
	return 0;
}
