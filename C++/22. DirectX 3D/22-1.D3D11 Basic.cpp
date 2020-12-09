#define WIN32_LEAN_AND_MEAN
// lean: ������ ����, �þ���, �������� ����
// mean: �������
// #define WIN32_LEAN_AND_MEAN: WIN32 �߿��� �ʿ��� ���(������ �͵�)�鸸 �����´ٴ� �ǹ�. ������ ��� ���� ���ǵǾ� ������, �������� �� ���� ���ȴ�.

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

using namespace Microsoft::WRL;

const wchar_t gClassName[]{ L"MyWindowClass" };
const wchar_t gTitle[]{ L"Direct3D" };
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

bool gMinimized{ false };	// �ּ�ȭ ���� ����
bool gMaximized{ false };	// �ִ�ȭ ���� ����
bool gResizing{ false };	// ������ ���� ������ ����

HWND						gHwnd{};
HINSTANCE					gInstance{};
ComPtr<ID3D11Device>		gspDevice{};		// ��ġ(�׷��� ī��)�� ���� ����(Transform, Rasterizer, Lighting). �׷���ī�� �޸𸮿� ���� �����ϴ� ��Ҵ� ���⼭ �����Ѵ�.
ComPtr<ID3D11DeviceContext>	gspDeviceContext{};	// ��ġ�� �����ϴ� ���. ��Ʈ�� ���� ����, ����, ��� ��� ���� �������� �������� ��� �ִ�. DirectX�� ���������� �� �߻�ȭ�� ������ ���⼭ �����Ѵ�.
ComPtr<IDXGISwapChain>		gspSwapChain{};
// DXGI(DirectX Graphics Infrastructure)
// : ���� ������� �׷���ī���, ���÷��� ��带 �����ϴ� �� DirectX�� ����ϱ� ���� ��� �����.
// ���ſ� DirectX�� ���ԵǾ� �־�����, �׸��� �׸��� �κи� ����� DXGI�� �и��ߴ�. DirectX 10, 11, 12���� ���ȴ�.

// Resource: �������� �ڷḦ �����ϴ� ����. ���� ������ ������.
ComPtr<ID3D11Texture2D>		gspRenderTarget{};
ComPtr<ID3D11Texture2D>		gspDepthStencil{};	// ����/���ٽ� ���۴� ���� ������ �������� �����Ƿ�, �ϳ��� �ڷ����� ��Ʈ�� ������ �����ϸ� ȿ�����̴�.

// View: ���ҽ����� ���ϴ� ������ �����ϱ� ���� Ư�� ������ ������.
ComPtr<ID3D11RenderTargetView> gspRenderTargetView{};	// ���� �ؽ���2D���� ������ �ڷῡ�� ���� Ÿ�� ������ ������ View
ComPtr<ID3D11DepthStencilView> gspDepthStencilView{};	// ����/���ٽ� ���� ������ �������� ���� View

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
	// ���� ȭ������ ä�� ���̹Ƿ�, ������ ����� ĥ�ϴ� �귯���� nullptr�� �����Ѵ�. (�귯���� ������ ������ �⺻ ����� �׸� �� ���� ȭ���� ������� �ȴ�)
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
	SetForegroundWindow(gHwnd);	// �� â�� ���� ������ ��������� �ǹ�. �켱������ �������Ƿ� ������ �ڿ��� ���� �Ἥ ������ �ε��� �� �ְ�, �������� ���� ������ �˸� �� �ִ�.
	SetFocus(gHwnd);			// �� â�� �Է��� �޴´ٴ� �ǹ�.
	UpdateWindow(gHwnd);

	// DirectX �ʱ�ȭ�� ������ ���� ����.
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
	// 1) ���� ü�� ����
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));	// Microsoft ǥ�ؿ��� ����ü ��� �� �׻� �޸𸮸� 0���� �ʱ�ȭ���� ��.

	// �ʼ��� �����ؾ� �ϴ� ��ҵ�
	scd.BufferCount = 1;								// �ĸ� ���� ����.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// ���� ü���� �����. ���⼭�� ���� Ÿ������ ����.
	scd.OutputWindow = gHwnd;
	scd.Windowed = TRUE;								// â ��� ����. true ����.

	// �߰�
	scd.SampleDesc.Count = 1;							// ���ø� ���
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// â ��� - ��ü ��� ����Ī ��� (DirectX ǥ��:Alt + Enter)
	scd.BufferDesc.Width = gScreenWidth;				// ����ü���� ���� ũ��(�ػ�) ����
	scd.BufferDesc.Height = gScreenHeight;
	
	D3D11CreateDeviceAndSwapChain(
		NULL,						// NULL�̸� ���� ������� �⺻ �׷���ī�带 �����´�.
		D3D_DRIVER_TYPE_HARDWARE,	// �ϵ���� �߻� ���̾�(HAL)�� �������� ���(Reference)�� ������, �������� �ϵ��� �״�� �� ������ ����. ���⼭�� ���ڷ� �׽�Ʈ.
		NULL,						// ��� �ڵ�. �ϵ��� �ٷ� �� ���� �ʿ����.
		NULL,						// �߰� �ɼ� ����. NULL�̸� �⺻ �ɼ�.
		NULL,						// �����ϴ� ���̷�ƮX�� ������ �迭�� �Է��Ѵ�. ���⼭�� D3D11�� �����ϴ� �Լ��̹Ƿ� DirectX 11�� ���� �����鸸 �����Ѵ�. NULL�̸� �⺻��.
		0,							// �� �迭�� ����
		D3D11_SDK_VERSION,			// SDK ����
		&scd,						// _DESC(description)�� ������ ��ҵ��� ������ Ŭ���� ���ó��, �������� ����ü�� �Է��� �� �ּҸ� �Ѱܴ޶�� �ǹ�.
		gspSwapChain.GetAddressOf(),
		gspDevice.GetAddressOf(),
		NULL,						// ���� �Է��� �����ϴ� ���̷�ƮX ������ �߿���, ������ ����� ��ǻ�Ϳ��� ��밡���� �������� �Է��ϱ� ���� ������ �Ѱ��ش�.
		gspDeviceContext.GetAddressOf()
	);

	// 2) ~ 5) OnResize ���� ó��.
	OnResize();
}

void OnResize()
{
	// 0) (���� ������ �ƴ� ���) �ػ� ���� �� ����� ��ҵ��� ���, ������ ���� ��������� �Ѵ�.

	// �ٿ� �� �ĸ� ���ۿ� ����-���ٽ� ���۸� �����Ѵ�. �� ���� �־��ָ� �ȴ�.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	gspDeviceContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);	// _countof()�� �迭�� ũ�⸦ ���� �� �ִ�. C�� ���Ե� ���.

	// ��� ���۸� �����Ѵ�.
	gspRenderTargetView.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspDepthStencil.Reset();

	// CPU�� �Լ��� �����ϰ� �۾� ����� ť�� ���� ������ �־�θ�, GPU�� �ش� �۾��� �ϳ��� �������� ������ �����Ѵ�.
	// ���� �ػ� ���� �� ������ ���� �۾����� �����, ���۸� �ʱ�ȭ�ؾ� �Ѵ�.
	gspDeviceContext->Flush();	// ������ ���� �۾� �����.
	
	
	// 1) ���� ü���� ���� ũ��(�ػ�) ���� (ResizeBuffers)
	//	ù ��° ���ڴ� ���� �����ε�, 0�� �Է��ϸ� ������ ������ �״�� ����Ѵ�. 2,3��°�� �ػ��� ����/���θ� �Է��Ѵ�.
	//	4,5��°�� �����̳� �÷��׸� �����ϴµ�, 0(UNKNOWN) �Է� �� DXGI�� �˾Ƽ� �⺻������ ����Ѵ�.
	gspSwapChain->ResizeBuffers(0, gScreenWidth, gScreenHeight, DXGI_FORMAT_UNKNOWN, 0);

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	// 2) ���� Ÿ�� ����. �̹� ����ü���� ������ ������ �װ� �����´�.	
	// ���� ü�ο��� ù ��° ����(���⼭�� �ĸ� ���� 1���̹Ƿ� �ش� ���۰� �� ���̴�)�� ���� Ÿ�ٿ� �����´�. �� ������� ���� Ÿ���� �ĸ� ���۰� �ȴ�.
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.GetAddressOf()));
	// �� ������ ������ �� define�� ���� �Ʒ��� ġȯ�ȴ�.
	//	gspSwapChain->GetBuffer(0, IID_ID3D11RenderTargetView, (void**)gspRenderTarget.GetAddressOf());

	// ���� Ÿ�� ���ҽ��κ��� ���� Ÿ�ٿ� �ʿ��� �ڷḦ �е��� View ������ �����Ѵ�. DESC�� ������ �����Ѱ� �״�� ���״� nullptr�� ����.
	gspDevice->CreateRenderTargetView(gspRenderTarget.Get(), nullptr, gspRenderTargetView.GetAddressOf());


	// 3) ����-���ٽ� ����
	// �ؽ��� 2D ������ �����ϴ� ����ü�� �����. Ÿ�� �տ� C�� ���̸� Ŭ������ ��ȯ, �����ڸ� ���� ���� �� �ִ�.
	CD3D11_TEXTURE2D_DESC dtd(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		gScreenWidth, gScreenHeight,
		1U, 1U,		// mipmap�� 0���� ���� �� ������ ��� ũ�⸦ �����.
		D3D11_BIND_DEPTH_STENCIL
	);
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(D3D11_DSV_DIMENSION_TEXTURE2D);

	// �ؽ��� ����, �ʱ�ȭ ������, ���� ������ �Ѱ��ش�. �ʱ�ȭ �����ʹ� ȭ�鿡 �׸� ��� ���� �޶����״� nullptr�� �Է��Ѵ�.
	gspDevice->CreateTexture2D(&dtd, nullptr, gspDepthStencil.GetAddressOf());
	gspDevice->CreateDepthStencilView(gspDepthStencil.Get(), &dsvd, gspDepthStencilView.GetAddressOf());


	// 4) �ĸ� ���ۿ� ����-���ٽ� ���� ��ġ��. ������ ������������ Output Merger �ܰ迡�� ó���ǹǷ� OM���� �����Ѵ�.
	gspDeviceContext->OMSetRenderTargets(
		1,									// ���� Ÿ�� ������ �Է��Ѵ�. �ſ��̳� ���� �� ��ġ�� ȿ���� ���ؼ��� ���� ���� ���� Ÿ���� �ʿ��� �� �ִ�. ���⼭�� �ش������ 1.
		gspRenderTargetView.GetAddressOf(), // ���� �����͸� �迭�� �޴´�. ���⼭�� 1���̹Ƿ� �� �ּҸ� �ѱ�� �ȴ�.
		gspDepthStencilView.Get()
	);


	// 5) �� ��Ʈ ����. 3D ������ 2D�� �����ϴ� �������� ���̹Ƿ� �����Ͷ��������� ���ȴ�. ������ RS�� �ٴ´�.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(gScreenWidth), static_cast<float>(gScreenHeight));
	gspDeviceContext->RSSetViewports(1, &viewport);	// �� ��Ʈ�� ������ ������ ����Ʈ�� �ּҸ� �ѱ��.
}

void DestroyD3D()
{
	// ��ü ȭ���� ���ü���� ���α׷� ���� ������ �׸��� �����Ƿ�, ��ü ȭ���� ��� ���� �� â ���� ��ȯ�ؾ� �Ѵ�.
	// SetFullscreenState: ù ��° ���ڴ� Ǯ��ũ�� ����, �� ��° ���ڴ� ���� ����͸� �� ��� Ǯ��ũ���� ����ͺ��� ���� ������ �� ����Ѵ�. ���⼭�� ����� 1���̹Ƿ� nullptr�� �⺻�� ���.
	gspSwapChain->SetFullscreenState(FALSE, nullptr);
	
	// ����Ʈ �������̹Ƿ� �˾Ƽ� ����������, ������ ���� ��������� �����Ѵ�.
	gspDepthStencil.Reset();
	gspDepthStencilView.Reset();
	gspRenderTarget.Reset();
	gspRenderTargetView.Reset();
	
	gspSwapChain.Reset();
	gspDevice.Reset();
	gspDeviceContext.Reset();

	DestroyWindow(gHwnd);
	UnregisterClass(gClassName, gInstance);	// �����쿡 ��ϵ� Ŭ������ �����. ���α׷��� ����Ǿ Ŭ���� ������ �����ֱ� ����.
}

void RenderFrame()
{
	// 3D�� �׸� �� ���� Ÿ��(�� ����)�� ���ٽ�-���� ���۸� ��� �ʱ�ȭ(Clear)�ؾ� �Ѵ�.
	float clearColor[4]{ 0.f, 0.2f, 0.4f, 1.f };
	gspDeviceContext->ClearRenderTargetView(gspRenderTargetView.Get(), clearColor);
	gspDeviceContext->ClearDepthStencilView(gspDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	// ����/���ٽ� ���� ��� �ʱ�ȭ
		1.0f,										// ���� ���� �ʱ�ȭ��: 1.0f (����������)
		0											// ���ٽ� ���� �ʱ�ȭ��: 1 (��� �׸���)
	);

	// Present�� ���� ü�ο��� ���� �׸��� �׸��� �� �� �ִ�.
	// ù ��° ���ڴ� ����� �ֻ������� ����ȭ(���� ����ȭ) ����. 0�̸� ����ȭ���� �ʰ�(�׸� �� �ִ� ��ŭ ��� �׸�, �׾ �߻� ����), n(1~4)�� ��� n���� �� ���۰� �׷��� ������ ����Ѵ�.
	// �� ��°�� �߰� �ɼ�. ������� ������ 0.
	gspSwapChain->Present(0, 0);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			// ������ �⺻ �귯���� �������Ƿ�, ������ ���� �� �׸��� �������� �׷���� �Ѵ�.
			if (gResizing)
			{
				// ������ ���� ���̸� �׸��� ��� �׸��� �Ѵ�.
				RenderFrame();
			}
			else
			{
				// �� ���� ���(�ּ�/�ִ�ȭ ��)���� ������ �⺻ �׸���(��� �ٿ� �׵θ� ��)�� �����Ѵ�.
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
			break;
		
		// â ũ�� ���� �� ȣ�� (â�� �ٸ� �ػ��� ����ͷ� �ű�� ���� ��쿡�� �߻�)
		case WM_SIZE:
			gScreenWidth = LOWORD(lParam);	// LParam�� ������ ��Ʈ(LOW)���� ���� ���̰�, ����(HIGH) ��Ʈ���� ���� ���̰� ���´�.
			gScreenHeight = HIWORD(lParam);
			if (gspDevice)	// �� �޽����� ���ʿ� �����찡 ������ ���� ȣ��ǹǷ�, Direct3D �ʱ�ȭ ��(Device�� ���� ��)���� OnResize() ó���� �ϸ� �� �ȴ�.
			{
				if (wParam == SIZE_MINIMIZED)	// wParam���� ũ�� ���� ������ ���´�.
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
						// ������ ���� �߿��� �� ������ ���� Ÿ���� ������ϸ� ���ϰ� ũ�� ������, �ƹ� ���۵� ���� �ʴ´�.
					}
					else
					{
						OnResize();	// ������ ���� ���� �ƴѵ� ȣ��� ��쿡�� OnResize�� ȣ���ϰ� �ؾ� �����ϴ�.
					}
				}
			}
			// ����) �ش� ó�� �� ������ �⺻ �޽��� ó��(DefWindowProc)���� �ߺ����� �ϸ� ������ ���� �� �ִ�.
			//		 DefWindowProc�� default�� �ΰų�, ���⼭ break ��� return�� ��Ű�� ������� ������ �� �ִ�.
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

		// WM_GETMINMAXINFO �޽����� â ũ�� ���� �� ȣ��Ǹ�, lParam�� ������ MINMAXINFO ������ ���·� ��ȯ�Ͽ�, �ּ� ũ�⸦ �������� �� �ִ�.
		// ������ ũ�� ���Ϸ� �����Ϸ� �ϸ�, ������ ũ��(���⼱ 640*480)�� �����ȴ�.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 480;
			break;

		// Alt Ű�� �޴��� �������� ��� ȣ���. (Alt + Enter�� ��ü-â ��� ���� �� ���� ���尡 ���� �� ����)
		// ��ȯ���� LRESULT�� long Ÿ������, ����(HIGH) 2����Ʈ�� ó�� ���(0�̸� ���������� ó����), ������ 2����Ʈ�� ó���� ������ ��ȯ�Ѵ�.
		// MAKELRESULT(0, MNC_CLOSE)�� �޴� â �ݱ⸦ ���������� ó���ߴٰ� �Ѱ��� �� �ִ�.
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
