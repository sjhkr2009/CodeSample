#include <Windows.h>
#include <d2d1.h>
#include <math.h>
#include <wrl.h>

#pragma comment (lib, "d2d1.lib")

const wchar_t gClassName[] = L"MyWindowClass";

// ComPtr: ���������� shared_ptr�� �����ϴ� COM ��ü�� ���� ����Ʈ ������.
//	��	����� ������ ������ �����Ƿ�, �����͸� �Ű������� �޴� �Լ��� ���� ��� ComPtr Ÿ������ �޾Ƽ� ���簡 �Ͼ�� ���� ī��Ʈ�� 1 �����Ѵ�.
//		�ش� ������ �����Ϸ��� �Ϲ� �����ͷ� ��ȯ(.Get())�Ͽ� �ްų�, ������(ComPtr&)���� �޾ƾ� �Ѵ�.

// ComPtr ����
Microsoft::WRL::ComPtr<ID2D1Factory> gspD2DFactory;
ID2D1HwndRenderTarget* gpRenderTarget{};

ID2D1SolidColorBrush* gpBrush{};
ID2D1RadialGradientBrush* gpRadialBrush{};

void ErrorBox(LPCWSTR errorText) { MessageBox(nullptr, errorText, L"Error", MB_OK); }
void OnPaint(HWND hwnd);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 1. ���丮 ����
	// ����Ʈ �����ʹ� .GetAddressOf()�� �ּҸ� �����´�.
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, gspD2DFactory.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorBox(L"Failed to Create Direct2D Factory");
		return 1;
	}
	
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

	// 2. ���� Ÿ�� ����
	// ����Ʈ �����͵� ������ �����ϴ�. (ȭ��ǥ ������ ���)
	// ������ ��ü�� �ѱ� ���� .Get() �� ����Ͽ� �Ϲ� �����ͷ� ĳ������ �� �ִ�.
	hr = gspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		&gpRenderTarget
	);

	if (FAILED(hr))
	{
		ErrorBox(L"Failed to Create D2D HWND Render Target!");
		return 1;
	}

	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0x9ACD32, 0.4f),
		&gpBrush
	);

	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];

	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f);
	gradientStops[0].position = 0.0f;

	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1.0f);
	gradientStops[1].position = 1.0f;

	hr = gpRenderTarget->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);

	if (SUCCEEDED(hr))
	{
		hr = gpRenderTarget->CreateRadialGradientBrush(
			D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(400, 400),
				D2D1::Point2F(0, 0),
				100, 100
			),
			pGradientStops,
			&gpRadialBrush
		);
		
		if (pGradientStops != nullptr)
		{
			pGradientStops->Release();
			pGradientStops = nullptr;
		}
	}

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
			OnPaint(hwnd);
		}
	}

	// 3. ��� ����
	if (gpRadialBrush != nullptr)
	{
		gpRadialBrush->Release();
		gpRadialBrush = nullptr;
	}
	if (gpBrush != nullptr)
	{
		gpBrush->Release();
		gpBrush = nullptr;
	}
	if (gpRenderTarget != nullptr)
	{
		gpRenderTarget->Release();
		gpRenderTarget = nullptr;
	}

	// ����Ʈ �������̹Ƿ� �ڵ����� �����ȴ�. ���������� �������ְ� ������ .Reset() �� ����Ѵ�.
	gspD2DFactory.Reset();
	gspD2DFactory = nullptr;

	return static_cast<int>(msg.wParam);
}

float angle = 0.f;

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	D2D1::ColorF bgColor = D2D1::ColorF(0.f, 0.f, 0.f);
	D2D1::ColorF blue = D2D1::ColorF(D2D1::ColorF::Aquamarine);
	D2D1::ColorF red = D2D1::ColorF(D2D1::ColorF::IndianRed);
	D2D1::ColorF yellow = D2D1::ColorF(D2D1::ColorF::Yellow);

	hdc = BeginPaint(hwnd, &ps);
	//------------------------------------------------

	// 4. �׸��� (��� �׸���� ���� Ÿ���� �Ѵ�)
	
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(bgColor);

	float moveCenter{ 75.f };
	float moveSpeed{ 0.07f };

	gpBrush->SetColor(yellow);
	gpBrush->SetOpacity(1.0f);
	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(300.f + moveCenter * (sinf(angle++ * moveSpeed)), 50.f), 50.f, 50.f),
		gpBrush
	);
	
	//------------------------------------------------

	gpBrush->SetColor(blue);
	gpRenderTarget->FillRectangle(
		D2D1::RectF(0.f, 0.f, 100.f, 100.f),
		gpBrush
	);

	gpBrush->SetOpacity(0.75f);
	gpBrush->SetColor(red);
	gpRenderTarget->FillRectangle(
		D2D1::RectF(50.f, 50.f, 150.f, 150.f),
		gpBrush
	);

	//--------------------------------------------------

	gpBrush->SetColor(blue);
	gpBrush->SetOpacity(0.8f);
	gpRenderTarget->FillRectangle(
		D2D1::Rect(300.f, 300.f, 500.f, 500.f),
		gpBrush
	);

	gpBrush->SetColor(bgColor);
	gpBrush->SetOpacity(1.0f);
	gpRenderTarget->FillRectangle(
		D2D1::Rect(310.f, 310.f, 490.f, 490.f),
		gpBrush
	);

	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(400.f, 400.f), 85.f, 85.f),
		gpRadialBrush
	);

	gpRenderTarget->EndDraw();
	//------------------------------------------------
	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			OnPaint(hwnd);
			break;

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