#define WIN32_LEAN_AND_MEAN
// lean: 군살이 없는, 늘씬한, 군더더기 없는
// mean: 평균적인
// #define WIN32_LEAN_AND_MEAN: WIN32 중에서 필요한 요소(가벼운 것들)들만 가져온다는 의미. 윈도우 헤더 내에 정의되어 있으며, 게임제작 시 자주 사용된다.

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

using namespace Microsoft::WRL;

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D" };
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

bool gMinimized{ false };	// 최소화 상태 여부
bool gMaximized{ false };	// 최대화 상태 여부
bool gResizing{ false };	// 사이즈 변경 중인지 여부

HWND						gHwnd{};
HINSTANCE					gInstance{};
ComPtr<ID3D11Device>		gspDevice{};		// 장치(그래픽 카드)에 대한 정보(Transform, Rasterizer, Lighting). 그래픽카드 메모리에 직접 접근하는 요소는 여기서 설정한다.
ComPtr<ID3D11DeviceContext>	gspDeviceContext{};	// 장치를 설명하는 기능. 폰트나 선의 굵기, 색상, 출력 방법 등의 세부적인 설정값이 들어 있다. DirectX의 파이프라인 등 추상화된 개념은 여기서 설정한다.
ComPtr<IDXGISwapChain>		gspSwapChain{};
// DXGI(DirectX Graphics Infrastructure)
// : 현재 사용중인 그래픽카드들, 디스플레이 모드를 열거하는 등 DirectX를 사용하기 위한 기반 기술들.
// 과거엔 DirectX에 포함되어 있었으나, 그림을 그리는 부분만 남기고 DXGI로 분리했다. DirectX 10, 11, 12에서 사용된다.

// Resource: 범용으로 자료를 저장하는 공간. 공통 포맷의 데이터.
ComPtr<ID3D11Texture2D>		gspRenderTarget{};
ComPtr<ID3D11Texture2D>		gspDepthStencil{};	// 깊이/스텐실 버퍼는 많은 정보를 저장하지 않으므로, 하나의 자료형에 비트를 나누어 저장하면 효율적이다.

// View: 리소스에서 원하는 정보만 추출하기 위한 특정 포맷의 데이터.
ComPtr<ID3D11RenderTargetView> gspRenderTargetView{};	// 위의 텍스쳐2D에서 저장한 자료에서 렌더 타겟 정보만 가져올 View
ComPtr<ID3D11DepthStencilView> gspDepthStencilView{};	// 깊이/스텐실 버퍼 정보를 가져오기 위한 View

void InitD3D();
void OnResize();
void DestroyD3D();
void RenderFrame();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	gInstance = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = gInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// 게임 화면으로 채울 것이므로, 윈도우 배경을 칠하는 브러쉬는 nullptr로 지정한다. (브러쉬가 있으면 윈도우 기본 배경을 그린 후 게임 화면을 덧씌우게 된다)
	wc.hbrBackground = nullptr;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed To Register!", L"Error", MB_OK);
		return 0;
	}

	RECT wr{ 0, 0, gScreenWidth, gScreenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	gHwnd = CreateWindowEx(
		NULL,
		gClassName,
		gTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL,
		NULL,
		gInstance,
		NULL
	);

	if (gHwnd == nullptr)
	{
		MessageBox(nullptr, L"Failed To Create Window!", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(gHwnd, nShowCmd);
	SetForegroundWindow(gHwnd);	// 이 창을 가장 앞으로 가져오라는 의미. 우선순위가 높아지므로 윈도우 자원도 많이 써서 빠르게 로딩할 수 있고, 유저에게 게임 실행을 알릴 수 있다.
	SetFocus(gHwnd);			// 이 창에 입력을 받는다는 의미.
	UpdateWindow(gHwnd);

	// DirectX 초기화는 윈도우 생성 이후.
	InitD3D();

	MSG msg{};
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
			// Game Loop
			RenderFrame();
		}
	}

	DestroyD3D();

	return static_cast<int>(msg.wParam);
}

void InitD3D()
{
	// 1) 스왑 체인 생성
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));	// Microsoft 표준에서 구조체 사용 시 항상 메모리를 0으로 초기화해줄 것.

	// 필수로 설정해야 하는 요소들
	scd.BufferCount = 1;								// 후면 버퍼 개수.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// 스왑 체인의 사용방식. 여기서는 렌더 타겟으로 지정.
	scd.OutputWindow = gHwnd;
	scd.Windowed = TRUE;								// 창 모드 여부. true 권장.

	// 추가
	scd.SampleDesc.Count = 1;							// 샘플링 배수
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// 창 모드 - 전체 모드 스위칭 허용 (DirectX 표준:Alt + Enter)
	scd.BufferDesc.Width = gScreenWidth;				// 스왑체인의 버퍼 크기(해상도) 지정
	scd.BufferDesc.Height = gScreenHeight;
	
	D3D11CreateDeviceAndSwapChain(
		NULL,						// NULL이면 현재 사용중인 기본 그래픽카드를 가져온다.
		D3D_DRIVER_TYPE_HARDWARE,	// 하드웨어 추상 레이어(HAL)를 가상으로 사용(Reference)할 것인지, 물리적인 하드웨어를 그대로 쓸 것인지 결정. 여기서는 후자로 테스트.
		NULL,						// 모듈 핸들. 하드웨어를 바로 쓸 때는 필요없음.
		NULL,						// 추가 옵션 설정. NULL이면 기본 옵션.
		NULL,						// 지원하는 다이렉트X의 버전을 배열로 입력한다. 여기서는 D3D11을 생성하는 함수이므로 DirectX 11의 세부 버전들만 지원한다. NULL이면 기본값.
		0,							// 위 배열의 개수
		D3D11_SDK_VERSION,			// SDK 버전
		&scd,						// _DESC(description)로 끝나는 요소들은 윈도우 클래스 등록처럼, 정보들을 구조체에 입력한 후 주소를 넘겨달라는 의미.
		gspSwapChain.GetAddressOf(),
		gspDevice.GetAddressOf(),
		NULL,						// 위에 입력한 지원하는 다이렉트X 버전들 중에서, 실행한 사용자 컴퓨터에서 사용가능한 버전들을 입력하기 위한 공간을 넘겨준다.
		gspDeviceContext.GetAddressOf()
	);

	// 2) ~ 5) OnResize 에서 처리.
	OnResize();
}

void OnResize()
{
	// 0) (최초 실행이 아닐 경우) 해상도 변경 시 변경될 요소들의 경우, 기존의 값은 삭제해줘야 한다.

	// 붙여 준 후면 버퍼와 깊이-스텐실 버퍼를 해제한다. 빈 값을 넣어주면 된다.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	gspDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);	// _countof()로 배열의 크기를 구할 수 있다. C언어에 포함된 기능.

	// 뷰와 버퍼를 리셋한다.
	gspRenderTargetView.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspDepthStencil.Reset();

	// CPU가 함수를 실행하고 작업 결과를 큐와 같은 공간에 넣어두면, GPU가 해당 작업을 하나씩 가져가는 식으로 동작한다.
	// 따라서 해상도 변경 시 기존에 쌓인 작업들을 지우고, 버퍼를 초기화해야 한다.
	gspDeviceContext->Flush();	// 기존에 쌓인 작업 지우기.
	
	
	// 1) 스왑 체인의 버퍼 크기(해상도) 변경 (ResizeBuffers)
	//	첫 번째 인자는 버퍼 개수인데, 0을 입력하면 원래의 개수를 그대로 사용한다. 2,3번째에 해상도의 가로/세로를 입력한다.
	//	4,5번째는 포맷이나 플래그를 지정하는데, 0(UNKNOWN) 입력 시 DXGI가 알아서 기본값으로 사용한다.
	gspSwapChain->ResizeBuffers(0, gScreenWidth, gScreenHeight, DXGI_FORMAT_UNKNOWN, 0);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	// 2) 렌더 타겟 설정. 이미 스왑체인이 가지고 있으니 그걸 가져온다.	
	// 스왑 체인에서 첫 번째 버퍼(여기서는 후면 버퍼 1개이므로 해당 버퍼가 될 것이다)를 렌더 타겟에 가져온다. 즉 여기부터 렌더 타겟이 후면 버퍼가 된다.
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.GetAddressOf()));
	// 위 동작은 컴파일 때 define에 의해 아래로 치환된다.
	//	gspSwapChain->GetBuffer(0, IID_ID3D11RenderTargetView, (void**)gspRenderTarget.GetAddressOf());

	// 렌더 타겟 리소스로부터 렌더 타겟에 필요한 자료를 읽도록 View 포맷을 설정한다. DESC는 위에서 설정한걸 그대로 쓸테니 nullptr을 쓴다.
	gspDevice->CreateRenderTargetView(gspRenderTarget.Get(), nullptr, gspRenderTargetView.GetAddressOf());


	// 3) 깊이-스텐실 버퍼
	// 텍스쳐 2D 정보를 저장하는 구조체를 만든다. 타입 앞에 C를 붙이면 클래스로 변환, 생성자를 통해 만들 수 있다.
	CD3D11_TEXTURE2D_DESC dtd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		gScreenWidth, gScreenHeight,
		1U, 1U,		// mipmap을 0으로 설정 시 가능한 모든 크기를 만들다.
		D3D11_BIND_DEPTH_STENCIL
	);
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

	// 텍스쳐 설정, 초기화 데이터, 저장 공간을 넘겨준다. 초기화 데이터는 화면에 그릴 대상에 따라 달라질테니 nullptr로 입력한다.
	gspDevice->CreateTexture2D(&dtd, nullptr, gspDepthStencil.GetAddressOf());
	gspDevice->CreateDepthStencilView(gspDepthStencil.Get(), &dsvd, gspDepthStencilView.GetAddressOf());


	// 4) 후면 버퍼와 깊이-스텐실 버퍼 합치기. 렌더링 파이프라인의 Output Merger 단계에서 처리되므로 OM으로 시작한다.
	gspDeviceContext->OMSetRenderTargets(
		1,									// 렌더 타겟 개수를 입력한다. 거울이나 수면 등 비치는 효과를 위해서는 여러 개의 렌더 타겟이 필요할 수 있다. 여기서는 해당없으니 1.
		gspRenderTargetView.GetAddressOf(), // 뷰의 포인터를 배열로 받는다. 여기서는 1개이므로 내 주소만 넘기면 된다.
		gspDepthStencilView.Get()
	);


	// 5) 뷰 포트 설정. 3D 공간을 2D로 투영하는 과정에서 쓰이므로 래스터라이저에서 사용된다. 약자인 RS가 붙는다.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(gScreenWidth), static_cast<float>(gScreenHeight));
	gspDeviceContext->RSSetViewports(1, &viewport);	// 뷰 포트의 개수와 설정된 뷰포트의 주소를 넘긴다.
}

void DestroyD3D()
{
	// 전체 화면일 경우운영체제가 프로그램 뒤의 내용은 그리지 않으므로, 전체 화면일 경우 종료 시 창 모드로 전환해야 한다.
	// SetFullscreenState: 첫 번째 인자는 풀스크린 여부, 두 번째 인자는 여러 모니터를 쓸 경우 풀스크린을 모니터별로 따로 제어할 때 사용한다. 여기서는 모니터 1개이므로 nullptr로 기본값 사용.
	gspSwapChain->SetFullscreenState(FALSE, nullptr);
	
	// 스마트 포인터이므로 알아서 해제되지만, 안전을 위해 명시적으로 리셋한다.
	gspDepthStencil.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspRenderTargetView.Reset();
	
	gspSwapChain.Reset();
	gspDevice.Reset();
	gspDeviceContext.Reset();

	DestroyWindow(gHwnd);
	UnregisterClass(gClassName, gInstance);	// 윈도우에 등록된 클래스도 지운다. 프로그램이 종료되어도 클래스 정보는 남아있기 때문.
}

void RenderFrame()
{
	// 3D를 그릴 땐 렌더 타겟(백 버퍼)과 스텐실-깊이 버퍼를 모두 초기화(Clear)해야 한다.
	float clearColor[4]{ 0.f, 0.2f, 0.4f, 1.f };
	gspDeviceContext->ClearRenderTargetView(gspRenderTargetView.Get(), clearColor);
	gspDeviceContext->ClearDepthStencilView(gspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// 깊이/스텐실 버퍼 모두 초기화
		1.0f,										// 깊이 버퍼 초기화값: 1.0f (검은색으로)
		0											// 스텐실 버퍼 초기화값: 1 (모두 그린다)
	);

	// Present로 스왑 체인에서 현재 그림을 그리게 할 수 있다.
	// 첫 번째 인자는 모니터 주사율과의 동기화(수직 동기화) 여부. 0이면 동기화하지 않고(그릴 수 있는 만큼 계속 그림, 테어링 발생 가능), n(1~4)일 경우 n개의 백 버퍼가 그려질 때까지 대기한다.
	// 두 번째는 추가 옵션. 사용하지 않으면 0.
	gspSwapChain->Present(0, 0);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			// 윈도우 기본 브러쉬를 지웠으므로, 사이즈 변경 시 그림을 수동으로 그려줘야 한다.
			if (gResizing)
			{
				// 사이즈 변경 중이면 그림을 계속 그리게 한다.
				RenderFrame();
			}
			else
			{
				// 그 외의 경우(최소/최대화 등)에는 윈도우 기본 그리기(상단 바와 테두리 등)를 실행한다.
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
			break;
		
		// 창 크기 변경 시 호출 (창을 다른 해상도의 모니터로 옮기는 등의 경우에도 발생)
		case WM_SIZE:
			gScreenWidth = LOWORD(lParam);	// LParam의 오른쪽 비트(LOW)에는 가로 길이가, 왼쪽(HIGH) 비트에는 세로 길이가 들어온다.
			gScreenHeight = HIWORD(lParam);
			if (gspDevice)	// 이 메시지는 최초에 윈도우가 생성될 때도 호출되므로, Direct3D 초기화 전(Device가 없을 때)에는 OnResize() 처리를 하면 안 된다.
			{
				if (wParam == SIZE_MINIMIZED)	// wParam에는 크기 조정 유형이 들어온다.
				{
					gMinimized = true;
					gMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					gMinimized = false;
					gMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (gMinimized)
					{
						gMinimized = false;
						OnResize();
					}
					else if (gMaximized)
					{
						gMaximized = false;
						OnResize();
					}
					else if (gResizing)
					{
						// 사이즈 변경 중에는 매 프레임 렌더 타겟을 재생성하면 부하가 크기 때문에, 아무 동작도 하지 않는다.
					}
					else
					{
						OnResize();	// 사이즈 변경 중이 아닌데 호출된 경우에는 OnResize를 호출하게 해야 안전하다.
					}
				}
			}
			// 참고) 해당 처리 후 윈도우 기본 메시지 처리(DefWindowProc)까지 중복으로 하면 문제가 생길 수 있다.
			//		 DefWindowProc를 default에 두거나, 여기서 break 대신 return을 시키는 방법으로 예방할 수 있다.
			return 0;
			break;
		
		case WM_ENTERSIZEMOVE:
			gResizing = true;
			return 0;
			break;

		case WM_EXITSIZEMOVE:
			gResizing = false;
			OnResize();
			return 0;
			break;

		// WM_GETMINMAXINFO 메시지는 창 크기 변경 시 호출되며, lParam의 정보를 MINMAXINFO 포인터 형태로 변환하여, 최소 크기를 지정해줄 수 있다.
		// 지정된 크기 이하로 변경하려 하면, 지정된 크기(여기선 640*480)로 고정된다.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
			break;

		// Alt 키로 메뉴에 접근했을 경우 호출됨. (Alt + Enter로 전체-창 모드 변경 시 에러 사운드가 나는 것 방지)
		// 반환값인 LRESULT는 long 타입으로, 왼쪽(HIGH) 2바이트는 처리 결과(0이면 정상적으로 처리됨), 오른쪽 2바이트는 처리한 내용을 반환한다.
		// MAKELRESULT(0, MNC_CLOSE)로 메뉴 창 닫기를 정상적으로 처리했다고 넘겨줄 수 있다.
		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);
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
