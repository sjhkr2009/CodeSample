#include "PCH.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DX;

// Microsoft::WRL 네임스페이스에 들어 있는 요소 중에서 ComPtr만 사용하겠다는 의미.
// 다른 변수나 함수들은 불러오지 않으므로 이름이 겹치는 것을 방지할 수 있다.
using Microsoft::WRL::ComPtr;

//__clang__: LLVM 컴파일 시 정의되어 있다. (비주얼 스튜디오는 __msvc__로 시작)
//			ㄴ 참고로 clang은 C언어(C Language)라는 의미.
#ifdef __clang__

// clang의 diagnostic(비주얼 스튜디오 인텔리센스와 같은 컴파일 도우미)이 발생시키는 특정 에러를 무시한다.
#pragma clang diagnostic ignored "-Wcovered-switch-default"	// switch문에서 모든 케이스를 서술하고도 default를 달면 clang에서 에러가 발생한다.
#pragma clang diagnostic ignored "-Wswitch-enum"			// switch문에서 enum의 특정 케이스를 서술하지 않으면 clang에서 에러가 발생한다.

#endif

// 위의 두 번째 에러는 비주얼 스튜디오에서도 발생한다. 이는 에러 코드를 지정하여 무시한다.
#pragma warning(disable:4061)

// 무명 이름 공간 (unnamed namespace)
// 이곳에 선언된 요소들(함수/변수/객체 등)은 모두 이 파일에서만 접근이 가능하다. 외부에서 알 수 없는 변수를 은폐하기 위해 사용된다.
namespace
{
	// 위의 #ifdef 표기와 동일한 기능.
	// _DEBUG는 디버그 모드로 실행 시 정의된다.
#if defined(_DEBUG)

	// 디버그 모드의 경우, 디버그용 디바이스 생성이 가능한지 확인해본다.
	inline bool SDKLayersAvailable() noexcept
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,				// 기본 어댑터 사용
			D3D_DRIVER_TYPE_NULL,	// 다이렉트X에 내장된 디버그용 드라이버 사용
			nullptr,				// 소프트웨어 사용하지 않음
			D3D11_CREATE_DEVICE_DEBUG,	// 디버그용 디바이스를 플래그로 지정
			nullptr,				// 피쳐 레벨 불필요
			0,						// 피쳐 레벨 불필요
			D3D11_SDK_VERSION,		// 현재 사용중인 SDK 버전
			nullptr,				// 디버그용 디바이스를 만들 수 있는지 확인하기 위한 함수이므로, 생성된 장치를 받지는 않는다.
			nullptr,
			nullptr
		);

		return SUCCEEDED(hr);
	}
#endif

	// 추후 HDR 지원 때 사용할 예정. 이미지의 색상 데이터만 유지하고 SRGB 색상 공간이라고 명시하는 부분은 제거한다.
	inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt) noexcept
	{
		switch (fmt)
		{
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:	return DXGI_FORMAT_R8G8B8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:	return DXGI_FORMAT_B8G8R8A8_UNORM;
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:	return DXGI_FORMAT_B8G8R8X8_UNORM;
			default:								return fmt;
		}
	}
}

DeviceResources::DeviceResources(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat, UINT backBufferCount, D3D_FEATURE_LEVEL minFeatureLevel, unsigned int flags) noexcept :
	m_screenViewport{},
	m_backBufferFormat(backBufferFormat),
	m_depthBufferFormat(depthBufferFormat),
	m_backBufferCount(backBufferCount),
	m_d3dMinFeatureLevel(minFeatureLevel),
	m_options(flags | c_FlipPresent),
	m_window(nullptr),
	m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
	m_outputSize{ 0,0,1,1 },
	// RGB 색상공간을 0~255로(=FULL) 사용하고, 감마 보정 2.2를 적용(=G22)하며, Sitting을 사용하지 않고(=NONE), BT709를 지원해야 한다(P709). HDTV 규격과 같다. 기본값이므로 사용자가 동적으로 변경할수도 있다.
	m_colorSpace(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709),
	m_deviceNotify(nullptr)
{
}

// 옵션과 플래그, 어댑터 정보와 버전을 확인한 후, 장치를 생성한다. 디버그 모드일 경우 추가로 디버깅 옵션을 지정한다.
void DeviceResources::CreateDeviceResources()
{
#pragma region 1. Microsoft 개발용 플래그 생성
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// 윈도우/XBOX는 리틀 엔디안 방식이 기본값이므로 BGRA를 지원하게 한다.

#if defined(_DEBUG)
	if (SDKLayersAvailable())
	{
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
	else
	{
		// 컴파일러의 출력 창에 메시지를 출력한다. (Visual Studio 전용)
		OutputDebugString(L"Warning: Direct3D Debug Device is not available (SDKLayersAvailable)\n");
	}
#endif // 디버그 모드일 경우 디버그용 디바이스 생성 가능 여부를 확인하고, 디버그용 플래그를 추가한다.

#pragma endregion

#pragma region 2. DXGI 팩토리 생성 및 옵션 지원 여부 확인

	CreateFactory();

	// 기기에서 지원하지 않는 옵션을 지정하면 장치 생성에 실패하므로, 지원하지 않을 경우 해당 기능을 꺼 준다.
	// OS 차원에서 확인한다. 지원하는 버전에 맞는 팩토리 버전으로 변환을 시도한 후, 변환에 성공하면 지원하는 것으로 간주한다.

	// 1) Allow Tearing
	if (m_options & c_AllowTearing)
	{
		BOOL allowTearing = FALSE;

		ComPtr<IDXGIFactory5> factory5;
		HRESULT hr = m_dxgiFactory.As(&factory5);
		if (SUCCEEDED(hr))
		{
			hr = factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing,
				sizeof(allowTearing)
			);
		}

		if (FAILED(hr) || !allowTearing)
		{
			m_options &= ~c_AllowTearing;

#if defined(_DEBUG)
			// Allow Tearing은 가변 프레임레이트 디스플레이로 불린다.
			// 59.7hz 같은 모니터는 2~300번 그려도 60번만 그려지므로 고정 프레임레이트가 되지만, 고주사율 모니터의 경우 가변 프레임레이트 형태가 되기 때문.
			// 참고로 가변 주파수 방식은 윈도우 1주년 기념 업데이트 때(2016년) 지원하기 시작했으므로, factory5로 변환해서 지원 여부를 확인한다.
			OutputDebugStringA("Warning: Variable refresh rate displays not supported");
#endif
		}
	}

	// 2) Enable HDR
	if (m_options & c_EnableHDR)
	{
		BOOL enableHDR = FALSE;

		ComPtr<IDXGIFactory5> factory5;
		if (FAILED(m_dxgiFactory.As(&factory5)))
		{
			m_options &= ~c_EnableHDR;
#if defined(_DEBUG)
			// HDR 지원을 위해서는 마찬가지로 윈도우 1주년 기념 업데이트 이후 버전이어야 하고, 모니터도 HDR을 지원해야 한다.
			// (모니터 지원 여부는 나중에 체크하기로 한다.)
			OutputDebugStringA("Warning: HDR Swapchain not supported");
#endif
		}
	}

	// 3) Flip Present 
	if (m_options & c_FlipPresent)
	{
		ComPtr<IDXGIFactory4> factory4;
		if (FAILED(m_dxgiFactory.As(&factory4)))
		{
			m_options &= ~c_FlipPresent;
#if defined(_DEBUG)
			// 윈도우10부터 지원된다. 1주년 기념 업데이트 전부터 지원하므로 factory4로 확인한다.
			// 모니터에 화면은 띄워야 하므로, Flip이 불가능하면 복사라도 하게 지정해야 함에 유의.
			// 참고로 마이크로소프트 스토어에 등록되는 앱들은 FLIP_DISCARD(FlipPresent와 동일. 플립 사용 및 사용된 버퍼는 버리는 옵션), FLIP_SEQUENCIAL(여러 버퍼를 이용할 경우, 순차적으로 출력) 중 하나를 반드시 지원해야 한다.
			OutputDebugStringA("Warning: HDR Swapchain not supported\n");
#endif
		}
	}

#pragma endregion

#pragma region 3. 버전 확인

	// 1) 일반적으로 다이렉트X 11 SDK로 만들었을때 지원하는 모든 버전을 나열한다.
	//	  이 중에서 우리가 만든 프로그램에서 지원하는 최소 버전에 따라 배열을 잘라서 사용할 것이다.
	static const D3D_FEATURE_LEVEL s_featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
		// 참고: 다이렉트 9.1부터 다이렉트10의 일부 기능을 윈도우7에서 사용가능하도록 추가했다.
		//		 9.0은 윈도우7 초창기 버전이고 많은 기능이 빠져있어서, 국산 게임은 보통 9.0c 또는 9.1부터 사용한다.
	};

	// 2) 이 프로그램이 지원하는 버전의 개수를 구한다. 최소사양보다 낮은 버전이 나올 때까지 카운팅한다.
	UINT featureLevelCount = 0;
	for (; featureLevelCount < _countof(s_featureLevels); ++featureLevelCount)
	{
		if (s_featureLevels[featureLevelCount] < m_d3dMinFeatureLevel)
			break;
	}
	// 지원하는 값이 없다면(=0개일 경우) 이 프로그램의 요구사양이 DirectX 11.1보다 높으므로 에러를 던진다.
	if (!featureLevelCount)
	{
		throw std::out_of_range("MinFeatureLevel is too high!");
	}

#pragma endregion

#pragma region 4. 장치 생성

	// 1) 어댑터를 생성하여 이 컴퓨터의 그래픽카드 및 디스플레이 정보를 받아온다.
	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(adapter.GetAddressOf());

	// 2) 우선 지역 변수를 선언해 장치를 생성한다. 성공 시 나중에 클래스의 멤버 변수에 배정할 것이다.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	// 3) 어댑터 정보가 있을 경우, 장치와 컨텍스트를 생성한다. 위에서 만든 어댑터 정보, 플래그, 지원 버전을 사용한다.
	HRESULT hr = E_FAIL;
	if (adapter)
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			s_featureLevels,
			featureLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&m_d3dFeatureLevel,
			context.GetAddressOf()
		);
	}

	// 4) 장치 생성 실패 시 릴리즈 모드면 에러를 출력, 디버그 모드면 WARP 모드로 장치 생성을 시도한다.
	//	  장치 생성 성공 시 어떤 모드든 바로 다음 단계로 넘어간다.

	// NDEBUG: std::assert를 막아 준다. 일반적으로 릴리즈 모드에서 사용되나, 디버그 모드에서도 정의될 수 있다.
	//	* 참고: 디버그/릴리즈 사이에도 여러 단계가 있으며, NDEBUG는 assert를 제거하는 옵션에 해당한다. 디버그 모드로 게임을 배포할 경우 NDEBUG를 포함한 일부 옵션을 끈다.
#if defined(NDEBUG)
	else
	{
		throw std::exception("No Direct3D hardware device found.");
	}
#else // 디버그 모드일 경우
	// 그래픽카드 없이 CPU가 연산을 해 주도록 생성한다. (WARP Device)
	// 참고: 마이크로소프트 스토어에 등록하려면, 그래픽카드가 없을 때 WARP로 생성하도록 지정해야 한다. (윈도우 7까지는 Reference로 불렸음)
	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(
			nullptr,				// 기본값 어댑터 사용
			D3D_DRIVER_TYPE_WARP,	// WARP 드라이버 사용
			nullptr,
			creationFlags,
			s_featureLevels,
			featureLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			& m_d3dFeatureLevel,
			context.GetAddressOf()
		);

		if (SUCCEEDED(hr))
		{
			OutputDebugStringA("Info: Direct3D Adapter Created by WARP\n");
		}
	}
#endif

	ThrowIfFailed(hr);

#pragma endregion

#pragma region 5. 디버깅 옵션 설정

	// 1) 디버그 메시지 관리 인터페이스 생성이 가능한지 확인한다.
	// 2) 메시지의 심각도에 따라 프로그램을 정지시키는 옵션을 지정한다.
	// 3) 불필요한 디버깅 메시지는 출력하지 않도록 설정한다.

#ifndef NDEBUG
	// std::assert만 제거한 디버그 모드일 경우에 설정하는 디버깅 옵션.
	// ID3D11Debug와 ID3D11InfoQueue로 모두 변환 가능한지 확인한다.

	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		// 디버그 메시지를 관리하는 인터페이스. 디버그 메시지를 가져오거나, 저장하거나, 단계별로 필터링할 수 있다.
		// 다이렉트X의 필터링 옵션을 변경 하는 용도의 인터페이스이므로, 옵션 지정 후 인터페이스는 해제해도 옵션은 유지된다. (ComPtr이므로 이 블럭이 끝나면 자동 해제)
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			// 메시지의 심각도에 따라 프로그램을 정지시킨다. (Break Point를 지정한 것처럼 멈춤)
			// 프로그램이 다운되거나 에러 발생 시 정지시키도록 설정한다. 디버그 모드에서만 정지 후 정보 확인이 가능하므로 전처리기를 사용한다.
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			// 디버그 메시지 중 불필요한 메시지는 출력하지 않도록 설정한다.
			// 여기선 메시지의 ID로 필터링하기로 한다.
			D3D11_MESSAGE_ID hide[] = {
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
			};

			D3D11_INFO_QUEUE_FILTER filter{};
			filter.DenyList.pIDList = hide;
			filter.DenyList.NumIDs = _countof(hide);
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

#pragma endregion

	// 위에서 생성한 디바이스 및 컨텍스트를 멤버 변수에 넘겨주고, 실패 시 예외 처리한다.
	ThrowIfFailed(device.As(&m_d3dDevice));
	ThrowIfFailed(context.As(&m_d3dDeviceContext));
	ThrowIfFailed(context.As(&m_d3dAnnotation));	// Annotation은 그래픽카드가 하는 일들의 정보를 들고 있으므로, 컨텍스트로부터 가져온다.
}

// 해상도 종속 리소스들을 생성 또는 재생성한다.
void DeviceResources::CreateWindowSizeDependentResources()
{
	// 윈도우가 없을 경우 예외 처리.
	if (!m_window)
	{
		throw std::exception("Call SetWindow with a valid Win32 window handle");
	}

#pragma region 1. 기존의 버퍼/뷰 초기화

	// 1-1) 디바이스 컨텍스트의 렌더 타겟 뷰를 null로 설정
	ID3D11RenderTargetView* nullViews[]{ nullptr };
	m_d3dDeviceContext->OMGetRenderTargets(_countof(nullViews), nullViews, nullptr);

	// 1-2) 렌더 타겟과 깊이-스텐실 버퍼 초기화
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_renderTarget.Reset();
	m_depthStencil.Reset();

	// 1-3) DirectX 내부의 큐에 쌓인 작업을 그래픽카드가 한꺼번에 가져가서 하도록 한다.
	//	 ㄴ 위에서 명령한 OMGetRenderTargets 작업도 큐에 쌓이므로, 쌓인 작업을 모두 처리한 후 마지막으로 렌더 타겟을 비우게 된다.
	m_d3dDeviceContext->Flush();

#pragma endregion

#pragma region 2. 백 버퍼 크기 설정

	// 2-1) 잘못된 값이 들어가지 않도록 1 이상의 양수로 지정한다. std::min/max<type>은 상수를 반환한다.
	const uint backBufferWidth = std::max<uint>(static_cast<uint>(m_outputSize.right - m_outputSize.left), 1u);
	const uint backBufferHeight = std::max<uint>(static_cast<uint>(m_outputSize.bottom - m_outputSize.top), 1u);

	// 2-2) 윈도우10 이상(factory4 이상)에서 지원하는 스왑체인 옵션 중 하나 이상이 켜져 있다면, sRGB를 사용하지 않도록 설정한다.
	DXGI_FORMAT backBufferFormat = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ?
		NoSRGB(m_backBufferFormat) : m_backBufferFormat;

#pragma endregion

#pragma region 3. 스왑체인 생성 또는 재설정

	HRESULT hr;

	// 3-1) 스왑체인이 이미 있는 경우, 사이즈만 재설정한다.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(
			m_backBufferCount,
			backBufferWidth,
			backBufferHeight,
			backBufferFormat,
			(m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
		);
		
		// DXGI_ERROR_DEVICE_RESET: 그래픽카드가 연산 수행에 실패하면, 시스템이 다운되는 것을 방지하기 위해 그래픽카드를 초기화한다.
		//							이 때 쌓인 작업은 초기화되며 Reset 메시지를 반환한다.
		//							(최신 그래픽카드들은 내부적으로 이런 로직을 갖고 있음)
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
#ifdef _DEBUG
			// 디버그 모드인 경우 정보를 알려준다.
			// 그래픽카드가 제거된 상황은 운영체제가 정보를 알 수 있으므로 GetDeviceRemovedReason로 상세정보를 출력한다.
			// 그래픽카드가 알 수 없는 이유로 명령 수행에 실패한 Reset은 에러 코드만 출력한다.
			char buff[64]{};
			sprintf_s(buff, "Device Lost on ResizeBuffers : Reason Code 0x%08X\n",
				static_cast<uint>(hr == DXGI_ERROR_DEVICE_REMOVED) ?
				m_d3dDevice->GetDeviceRemovedReason() : hr
			);

			OutputDebugStringA(buff);
#endif
			// Device Lost 처리를 하고 return 한다.
			OnHandleDeviceLost();
			return;
		}
		else
		{
			// 그 외의 에러들은 여기서 처리할 수 없으니 예외 처리만 한다.
			ThrowIfFailed(hr);
		}
	}
	// 3-2) 스왑체인이 없는 경우, 새로 생성한다.
	else
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = m_backBufferCount;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;	// 화면 크기 확대 시 단순히 늘리는 설정. (ASPECT_RATIO_STRETCH 는 종횡비를 유지하며 늘린다. 빈 공간은 래터 박스로 채운다.)
		// FLIP_DISCARD는 플립 방식으로 출력하되 출력 후 백 버퍼는 지우는 방식, 일반 DISCARD는 버퍼 교체(flip)가 아닌 복사로 출력한다.
		// 최신 버전에서만 플립을 적용한다.
		swapChainDesc.SwapEffect = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ? 
			DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
		// 알파 모드와 알파값 출력 여부는 무관.
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = (m_options & c_AllowTearing) ?
			DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

		// 풀스크린 모드는 별도의 Description에서 설정한다.
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc{};
		fsSwapChainDesc.Windowed = TRUE;

		// CreateSwapChainForCoreWindow는 UWP(Universal Window)에서 윈도우를 생성한다. C#용이었으나 C++에서도 사용가능하게 변경되었다.
		// 여기서는 윈도우 프로그래밍 및 C++ 연습을 위해 CoreWindow가 아닌 Hwnd를 사용하여 생성한다.
		hr = m_dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,			// 여기 적힌 output에는 화면을 출력하지 않는다. 여기서는 지정하지 않는다.
			m_swapChain.ReleaseAndGetAddressOf()
		);
		ThrowIfFailed(hr);

		// 전체 화면 허용 시, 그래픽카드 자원을 게임이 독점하게 된다. 성능이 소폭 좋아질 수 있으나 전환 딜레이가 생겨, 장치 손실의 위험이 있다.
		// 전체 화면 창 모드 로 대체할 경우 바탕화면 리소스와 게임을 함께 출력하므로 성능이 소폭 하락할 수 있으나, 전환 딜레이가 없어진다.
		hr = m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER); // Alt + Enter(전체 화면) 키의 동작을 막는다.
		ThrowIfFailed(hr);
	}

	// 3-3) 색상 공간을 업데이트한다.
	UpdateColorSpace();

#pragma endregion

#pragma region 4. 렌더 타겟 뷰 생성

	// 4-1) 스왑 체인의 첫 번째 버퍼를 렌더 타겟에 가져온다.
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf()));
	ThrowIfFailed(hr);

	// 4-2) 타입(텍스쳐2D)과 포맷을 통해 Description을 설정하고 렌더 타겟 뷰를 생성한다.
	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
	hr = m_d3dDevice->CreateRenderTargetView(
		m_renderTarget.Get(),
		&renderTargetViewDesc,
		m_renderTargetView.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

#pragma endregion

#pragma region 5. 깊이-스텐실 버퍼 생성
	
	// 2D 게임에서는 필요없으니, DXGI_FORMAT_UNKNOWN일 때는 만들지 않는다.
	if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
	{
		// 5-1) 깊이-스텐실 버퍼 생성
		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			m_depthBufferFormat,
			backBufferWidth,
			backBufferHeight,
			1, 1,
			D3D11_BIND_DEPTH_STENCIL
		);
		hr = m_d3dDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			m_depthStencil.ReleaseAndGetAddressOf()
		);
		ThrowIfFailed(hr);

		// 5-2) 깊이-스텐실 뷰 생성
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		hr = m_d3dDevice->CreateDepthStencilView(
			m_depthStencil.Get(),
			&depthStencilViewDesc,
			m_depthStencilView.ReleaseAndGetAddressOf()
		);
		ThrowIfFailed(hr);
	}

#pragma endregion

	// 마지막으로 뷰 포트를 생성한다.
	m_screenViewport = CD3D11_VIEWPORT(
		0.0f, 0.0f,
		static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight)
	);
}

// 윈도우 창을 멤버 변수로 저장한다. 창 크기는 RECT로 변환하여 멤버 변수에 저장한다.
void DeviceResources::SetWindow(HWND window, int width, int height) noexcept
{
	m_window = window;

	m_outputSize.left = m_outputSize.right = 0;
	m_outputSize.right = width;
	m_outputSize.bottom = height;
}

// 윈도우 크기 변경 시 호출된다. 해상도 및 디스플레이만 바뀐 경우 색상 공간만 업데이트하고 false를 반환한다. 프로그램의 창 크기가 변경되면 해상도 종속 리소스들을 재생성하고 true를 반환한다.
bool DeviceResources::OnWindowSizeChanged(int width, int height)
{
	RECT newRc;
	newRc.left = newRc.top = 0;
	newRc.right = width;
	newRc.bottom = height;

	if (newRc == m_outputSize)
	{
		UpdateColorSpace();
		return false;
	}

	m_outputSize = newRc;
	CreateWindowSizeDependentResources();

	return true;
}

// 뷰와 버퍼 및 장치를 모두 초기화하고 다시 생성한다. 동작 전후 Device Notify에서 Device Lost/Restore를 보고한다. 디버그 모드일 경우 장치 초기화 전에 장치에 쌓인 디버그 정보를 모두 출력한다.
void DeviceResources::OnHandleDeviceLost()
{
	if (m_deviceNotify)
	{
		m_deviceNotify->OnDeviceLost();
	}

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_depthStencil.Reset();
	m_renderTarget.Reset();
	
	m_swapChain.Reset();
	m_d3dDeviceContext.Reset();
	m_d3dAnnotation.Reset();

#ifdef _DEBUG
	{
		ComPtr<ID3D11Debug> d3dDebug;
		if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
		{
			// ReportLiveDeviceObjects: Device에 지금까지 쌓여 있던 디버그 정보들을 모두 출력한다. (플래그로 전체 출력/요약 출력을 지정할 수 있다)
			d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
		}
	}
#endif

	m_d3dDevice.Reset();
	m_dxgiFactory.Reset();

	CreateDeviceResources();
	CreateWindowSizeDependentResources();

	if (m_deviceNotify)
	{
		m_deviceNotify->OnDeviceRestored();
	}
}

// 수직 동기화 여부를 확인한 후 스왑 체인의 Present를 실행하여 화면을 출력하고, 백 버퍼를 폐기한다.
void DeviceResources::Present()
{
	// 옵션의 수직 동기와 여부에 따라 지연시간을 설정, 스왑체인의 Present를 동작시킨다.
	HRESULT hr = (m_options & c_AllowTearing) ?
		m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING) : m_swapChain->Present(1, 0);

	// 작동 후 백 버퍼는 버린다. 깊이-스텐실 버퍼는 2D에선 생성하지 않으므로, 해당 버퍼가 있을 때만 버린다.
	m_d3dDeviceContext->DiscardView(m_renderTargetView.Get());
	if (m_depthStencilView)
	{
		m_d3dDeviceContext->DiscardView(m_depthStencilView.Get());
	}

	// 장치가 없거나 리셋된 경우 위와 동일하게, 디버그 정보 출력 후 OnHandleDeviceLost를 호출한다.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
#ifdef _DEBUG
		char buff[64]{};
		sprintf_s(buff, "Device Lost on ResizeBuffers : Reason Code 0x%08X\n",
			static_cast<uint>(hr == DXGI_ERROR_DEVICE_REMOVED) ?
			m_d3dDevice->GetDeviceRemovedReason() : hr
		);

		OutputDebugStringA(buff);
#endif
		OnHandleDeviceLost();
		return;
	}

	// 그 외의 에러는 예외 처리한다.
	ThrowIfFailed(hr);

	// IsCurrent: DXGI에 캐싱된 출력 정보가 유효한지(또는 오래되었는지) 확인한다. 그래픽 카드를 재생성해야 할 경우 false를 반환한다.
	//			ㄴ실제 이런 에러가 나는 경우는 주로 HDR 색상 모드 사용으로 인해 캐시 용량이 초과하는 상황이다. 그 외엔 거의 발생하지 않는다.
	if (!m_dxgiFactory->IsCurrent())
	{
		CreateFactory();
	}
}

// DXGI 팩토리를 생성한다. 디버그 모드일 경우 추가로 디버깅 옵션을 지정한다.
void DeviceResources::CreateFactory()
{
	HRESULT hr;
#if defined(_DEBUG) && (_WIN32_WINNT >= 0x0603)	// 0x0603: 윈도우 8.1
	bool debugDXGI = false;
	{
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;

		hr = DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(hr))
		{
			debugDXGI = true;

			hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
			ThrowIfFailed(hr);

			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

			// 80번 에러: 모니터가 2개 이상일 때, 실행중인 프로그램을 (출력 대상 모니터와) 다른 모니터로 이동했을 때 발생하는 에러.
			//			  스왑체인의 GetContainingOutput 호출 시 프로그램을 실행한 모니터와 현재 프로그램이 있는 모니터가 다를 경우 발생한다. 중요하지 않은 에러니 무시한다.
			DXGI_INFO_QUEUE_MESSAGE_ID hide[]{
				80
			};
			DXGI_INFO_QUEUE_FILTER filter{};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
		}
	}
	if (debugDXGI)
		return;
#endif

	hr = CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
	ThrowIfFailed(hr);
}

// 컴퓨터의 그래픽카드를 가져온다. 가장 성능이 좋은 GPU를 찾을 수 있다면 찾아오고, 아니라면 첫 번째 그래픽카드부터 탐색하여 가져온다.
void DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

	// 특정 버전 이상일 경우, 이 컴퓨터에서 가장 성능이 좋은 GPU를 찾아올 수 있다. (DXGI 버전 1.6 이상, 윈도우10 레드스톤 4 업데이트 이상에서 지원)
#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = m_dxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(	// 그래픽카드를 하나씩 가져온다. 더 이상 가져올 그래픽카드가 없을 때까지 실행한다.
				adapterIndex,							// 가져올 그래픽카드의 인덱스
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,	// 인덱스 정렬 기준. 여기선 고성능인 GPU가 먼저(= 낮은 인덱스에서) 나오게 지정한다.
				IID_PPV_ARGS(adapter.GetAddressOf())	// 가져온 그래픽카드 정보에 접근하기 위한 인터페이스
			));
			adapterIndex++)
		{
			// 해당 그래픽카드의 정보를 읽어서, 소프트웨어일 경우에는 다른 그래픽카드를 탐색한다.
			// * 소프트웨어 플래그는 윈도우 기본 드라이버(Microsoft Basic Render Driver)에서 반환된다.
			DXGI_ADAPTER_DESC1 desc;
			ThrowIfFailed(adapter->GetDesc1(&desc));
			
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// 디버그 모드일 경우 어떤 그래픽카드를 사용하는지 출력한다.
#ifdef _DEBUG
			wchar_t buff[256]{};
			// %04x 또는 %04X : 16진수 4자리 출력 (A~F의 경우 x는 소문자로, X는 대문자로)
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif
			// 가져온 그래픽카드 사용에 문제가 없다면 break
			break;
		}
	}
#endif

	// 위의 if문 진입에 실패하거나 그래픽카드 탐색에 실패한 경우, 맨 앞의 그래픽카드부터 하나씩 탐색한다.
	// 탐색된 그래픽카드에 대해서는 위와 동일한 처리.
	if (!adapter)
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(m_dxgiFactory->EnumAdapters1(
				adapterIndex,
				adapter.ReleaseAndGetAddressOf()));
			adapterIndex++
			)
		{
			DXGI_ADAPTER_DESC1 desc;
			ThrowIfFailed(adapter->GetDesc1(&desc));

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

#ifdef _DEBUG
			wchar_t buff[256]{};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif
			break;
		}
	}

	// unique/shared ptr의 소유권 이전은 std::move()를 사용해서 이동시키지만, ComPtr은 .Detach()를 이용한다.
	// 단순히 대입할 경우 adapter는 이 블럭이 끝나면 참조 카운트가 0이 되어 데이터의 메모리가 해제되므로 주의.
	*ppAdapter = adapter.Detach();
}

// 프로그램이 출력되는 모니터 정보를 찾아서 알맞은 색상 공간을 지정하고, 스왑 체인을 업데이트한다.
void DeviceResources::UpdateColorSpace()
{
	// HDTV의 표준 포맷
	DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

	bool isDisplayHDR10 = false;

	// HDR10 지원 여부 확인. 윈도우10 레드스톤2 업데이트부터 지원한다.
#if defined(NTDDI_WIN10_RS2)
	// 프로그램이 출력되는 모니터는, 프로그램이 더 많이 걸쳐 있는 모니터를 타겟으로 한다.
	// 이를 판단하기 위해서 스왑 체인은 필수적으로 있어야 한다.
	if (m_swapChain)
	{
		ComPtr<IDXGIOutput> output;
		// GetContainingOutput : 프로그램이 가장 많이 걸쳐 있는 모니터를 output에 가져온다.
		if (SUCCEEDED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
		{
			ComPtr<IDXGIOutput6> output6;
			if (SUCCEEDED(output.As(&output6)))
			{
				DXGI_OUTPUT_DESC1 desc;
				ThrowIfFailed(output6->GetDesc1(&desc));

				// 해당 모니터의 색상 공간이 HDR10 모델인 경우 isDisplayHDR10를 true로 바꿔준다. UHD(4K, 8K) 모니터에서 지원한다.
				if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
				{
					isDisplayHDR10 = true;
				}
			}
		}
	}
#endif

	// HDR 옵션이 켜져 있고, 이 모니터에서 HDR10을 지원한다면 색상 공간을 출력 이미지(백 버퍼 포맷)에 맞게 지정한다.
	if ((m_options & c_EnableHDR) && isDisplayHDR10)
	{
		switch (m_backBufferFormat)
		{
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
				break;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
				break;
		}
	}

	m_colorSpace = colorSpace;

	ComPtr<IDXGISwapChain3> swapChain3;
	if (SUCCEEDED(m_swapChain.As(&swapChain3)))
	{
		UINT colorSpaceSupport = 0;
		// CheckColorSpaceSupport: 색상 공간(colorSpace)이 해당 스왑체인에서 지원되는지 확인하여, 해당 정보를 colorSpaceSupport에 플래그로 세팅한다.
		if (SUCCEEDED(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)) &&
			(colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		{
			ThrowIfFailed(swapChain3->SetColorSpace1(colorSpace));
		}
	}
}
