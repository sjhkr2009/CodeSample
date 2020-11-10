#include <Windows.h>
#include <d2d1.h>
#include <math.h>	// 삼각함수를 사용하기 위한 헤더

#pragma comment (lib, "d2d1.lib")

const wchar_t gClassName[] = L"MyWindowClass";

ID2D1Factory* gpD2DFactory{};
ID2D1HwndRenderTarget* gpRenderTarget{};

// 브러쉬 인터페이스 선언
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
	// 1. 팩토리 생성
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gpD2DFactory);
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

	// 2. 렌더 타겟 생성
	hr = gpD2DFactory->CreateHwndRenderTarget(
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
	
	// 브러쉬 생성
	hr = gpRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(0x9ACD32, 0.4f),
		&gpBrush
	);

	// 그래디언트 브러쉬 생성
	//	 Gradient Stop의 정보를 먼저 지정해야 한다.
	ID2D1GradientStopCollection* pGradientStops{};
	D2D1_GRADIENT_STOP gradientStops[2];

	//	 D2D1::ColorF::의 enum에 정의된 값은 알파 값이 빠져 있으므로, 알파값은 따로 입력해야 한다.
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f);
	gradientStops[0].position = 0.0f;

	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Crimson, 1.0f);
	gradientStops[1].position = 1.0f;

	//	 gradientStops 배열과 개수, 감마 보정, 확장 모드를 입력하고, 마지막으로 GradientStops 배열 포인터의 포인터를 넣는다.
	hr = gpRenderTarget->CreateGradientStopCollection(
		gradientStops,
		2,
		D2D1_GAMMA_2_2,
		D2D1_EXTEND_MODE_CLAMP,
		&pGradientStops
	);

	// 그래디언스 스탑 생성에 성공한 경우에만 브러쉬를 생성. 성공 여부는 SUCCEEDED() 함수를 이용한다.
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
		// 브러쉬 생성 후에는 pGradientStops 포인터는 필요 없으니 해제해준다.
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
	// PeekMessage로 메시지가 있든 없든 매 프레임마다 실행되게 한다. 마지막 인자로는 메시지를 받으면 지울지 말지 선택한다.
	// PeekMessage는 메시지가 있으면 true, 없으면 false를 반환한다. 따라서 메시지 없으면 while문을 탈출해버리니 반복 조건은 그냥 true로 둔다.
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// while 조건이 true이므로, 프로그램 종료 시 탈출하도록 한다.
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			OnPaint(hwnd);				// 움직이는 원 테스트를 위해 메시지가 안 들어와도 그리기를 실행시킨다.
		}
	}

	// 3. 모두 해제
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
	if (gpD2DFactory != nullptr)
	{
		gpD2DFactory->Release();
		gpD2DFactory = nullptr;
	}

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

	// 4. 그리기 (모든 그리기는 렌더 타겟이 한다)
	
	// 렌더 타겟도 BeginDraw ~ EndDraw 사이에서 모두 처리한다.
	gpRenderTarget->BeginDraw();
	gpRenderTarget->Clear(bgColor);	// 배경 초기화

	// 포지션 변경하기 (250~300 반복)

	float moveCenter{ 75.f };
	float moveSpeed{ 0.07f };

	gpBrush->SetColor(yellow);
	gpBrush->SetOpacity(1.0f);
	gpRenderTarget->FillEllipse(
		D2D1::Ellipse(D2D1::Point2F(300.f + moveCenter * (sinf(angle++ * moveSpeed)), 50.f), 50.f, 50.f),
		gpBrush
	);
	
	//------------------------------------------------
	// 색상과 불투명도 조정

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
	// 아무거나 그려보기

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
