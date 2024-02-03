#include "PCH.h"
#include "DeviceResources.h"

using namespace DirectX;
using namespace DX;

// Microsoft::WRL ���ӽ����̽��� ��� �ִ� ��� �߿��� ComPtr�� ����ϰڴٴ� �ǹ�.
// �ٸ� ������ �Լ����� �ҷ����� �����Ƿ� �̸��� ��ġ�� ���� ������ �� �ִ�.
using Microsoft::WRL::ComPtr;

//__clang__: LLVM ������ �� ���ǵǾ� �ִ�. (���־� ��Ʃ����� __msvc__�� ����)
//			�� ����� clang�� C���(C Language)��� �ǹ�.
#ifdef __clang__

// clang�� diagnostic(���־� ��Ʃ��� ���ڸ������� ���� ������ �����)�� �߻���Ű�� Ư�� ������ �����Ѵ�.
#pragma clang diagnostic ignored "-Wcovered-switch-default"	// switch������ ��� ���̽��� �����ϰ� default�� �޸� clang���� ������ �߻��Ѵ�.
#pragma clang diagnostic ignored "-Wswitch-enum"			// switch������ enum�� Ư�� ���̽��� �������� ������ clang���� ������ �߻��Ѵ�.

#endif

// ���� �� ��° ������ ���־� ��Ʃ��������� �߻��Ѵ�. �̴� ���� �ڵ带 �����Ͽ� �����Ѵ�.
#pragma warning(disable:4061)

// ���� �̸� ���� (unnamed namespace)
// �̰��� ����� ��ҵ�(�Լ�/����/��ü ��)�� ��� �� ���Ͽ����� ������ �����ϴ�. �ܺο��� �� �� ���� ������ �����ϱ� ���� ���ȴ�.
namespace
{
	// ���� #ifdef ǥ��� ������ ���.
	// _DEBUG�� ����� ���� ���� �� ���ǵȴ�.
#if defined(_DEBUG)

	// ����� ����� ���, ����׿� ����̽� ������ �������� Ȯ���غ���.
	inline bool SDKLayersAvailable() noexcept
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,				// �⺻ ����� ���
			D3D_DRIVER_TYPE_NULL,	// ���̷�ƮX�� ����� ����׿� ����̹� ���
			nullptr,				// ����Ʈ���� ������� ����
			D3D11_CREATE_DEVICE_DEBUG,	// ����׿� ����̽��� �÷��׷� ����
			nullptr,				// ���� ���� ���ʿ�
			0,						// ���� ���� ���ʿ�
			D3D11_SDK_VERSION,		// ���� ������� SDK ����
			nullptr,				// ����׿� ����̽��� ���� �� �ִ��� Ȯ���ϱ� ���� �Լ��̹Ƿ�, ������ ��ġ�� ������ �ʴ´�.
			nullptr,
			nullptr
		);

		return SUCCEEDED(hr);
	}
#endif

	// ���� HDR ���� �� ����� ����. �̹����� ���� �����͸� �����ϰ� SRGB ���� �����̶�� ����ϴ� �κ��� �����Ѵ�.
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
	// RGB ��������� 0~255��(=FULL) ����ϰ�, ���� ���� 2.2�� ����(=G22)�ϸ�, Sitting�� ������� �ʰ�(=NONE), BT709�� �����ؾ� �Ѵ�(P709). HDTV �԰ݰ� ����. �⺻���̹Ƿ� ����ڰ� �������� �����Ҽ��� �ִ�.
	m_colorSpace(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709),
	m_deviceNotify(nullptr)
{
}

// �ɼǰ� �÷���, ����� ������ ������ Ȯ���� ��, ��ġ�� �����Ѵ�. ����� ����� ��� �߰��� ����� �ɼ��� �����Ѵ�.
void DeviceResources::CreateDeviceResources()
{
#pragma region 1. Microsoft ���߿� �÷��� ����
	
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// ������/XBOX�� ��Ʋ ����� ����� �⺻���̹Ƿ� BGRA�� �����ϰ� �Ѵ�.

#if defined(_DEBUG)
	if (SDKLayersAvailable())
	{
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
	else
	{
		// �����Ϸ��� ��� â�� �޽����� ����Ѵ�. (Visual Studio ����)
		OutputDebugString(L"Warning: Direct3D Debug Device is not available (SDKLayersAvailable)\n");
	}
#endif // ����� ����� ��� ����׿� ����̽� ���� ���� ���θ� Ȯ���ϰ�, ����׿� �÷��׸� �߰��Ѵ�.

#pragma endregion

#pragma region 2. DXGI ���丮 ���� �� �ɼ� ���� ���� Ȯ��

	CreateFactory();

	// ��⿡�� �������� �ʴ� �ɼ��� �����ϸ� ��ġ ������ �����ϹǷ�, �������� ���� ��� �ش� ����� �� �ش�.
	// OS �������� Ȯ���Ѵ�. �����ϴ� ������ �´� ���丮 �������� ��ȯ�� �õ��� ��, ��ȯ�� �����ϸ� �����ϴ� ������ �����Ѵ�.

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
			// Allow Tearing�� ���� �����ӷ���Ʈ ���÷��̷� �Ҹ���.
			// 59.7hz ���� ����ʹ� 2~300�� �׷��� 60���� �׷����Ƿ� ���� �����ӷ���Ʈ�� ������, ���ֻ��� ������� ��� ���� �����ӷ���Ʈ ���°� �Ǳ� ����.
			// ����� ���� ���ļ� ����� ������ 1�ֳ� ��� ������Ʈ ��(2016��) �����ϱ� ���������Ƿ�, factory5�� ��ȯ�ؼ� ���� ���θ� Ȯ���Ѵ�.
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
			// HDR ������ ���ؼ��� ���������� ������ 1�ֳ� ��� ������Ʈ ���� �����̾�� �ϰ�, ����͵� HDR�� �����ؾ� �Ѵ�.
			// (����� ���� ���δ� ���߿� üũ�ϱ�� �Ѵ�.)
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
			// ������10���� �����ȴ�. 1�ֳ� ��� ������Ʈ ������ �����ϹǷ� factory4�� Ȯ���Ѵ�.
			// ����Ϳ� ȭ���� ����� �ϹǷ�, Flip�� �Ұ����ϸ� ����� �ϰ� �����ؾ� �Կ� ����.
			// ����� ����ũ�μ���Ʈ ���� ��ϵǴ� �۵��� FLIP_DISCARD(FlipPresent�� ����. �ø� ��� �� ���� ���۴� ������ �ɼ�), FLIP_SEQUENCIAL(���� ���۸� �̿��� ���, ���������� ���) �� �ϳ��� �ݵ�� �����ؾ� �Ѵ�.
			OutputDebugStringA("Warning: HDR Swapchain not supported\n");
#endif
		}
	}

#pragma endregion

#pragma region 3. ���� Ȯ��

	// 1) �Ϲ������� ���̷�ƮX 11 SDK�� ��������� �����ϴ� ��� ������ �����Ѵ�.
	//	  �� �߿��� �츮�� ���� ���α׷����� �����ϴ� �ּ� ������ ���� �迭�� �߶� ����� ���̴�.
	static const D3D_FEATURE_LEVEL s_featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
		// ����: ���̷�Ʈ 9.1���� ���̷�Ʈ10�� �Ϻ� ����� ������7���� ��밡���ϵ��� �߰��ߴ�.
		//		 9.0�� ������7 ��â�� �����̰� ���� ����� �����־, ���� ������ ���� 9.0c �Ǵ� 9.1���� ����Ѵ�.
	};

	// 2) �� ���α׷��� �����ϴ� ������ ������ ���Ѵ�. �ּһ�纸�� ���� ������ ���� ������ ī�����Ѵ�.
	UINT featureLevelCount = 0;
	for (; featureLevelCount < _countof(s_featureLevels); ++featureLevelCount)
	{
		if (s_featureLevels[featureLevelCount] < m_d3dMinFeatureLevel)
			break;
	}
	// �����ϴ� ���� ���ٸ�(=0���� ���) �� ���α׷��� �䱸����� DirectX 11.1���� �����Ƿ� ������ ������.
	if (!featureLevelCount)
	{
		throw std::out_of_range("MinFeatureLevel is too high!");
	}

#pragma endregion

#pragma region 4. ��ġ ����

	// 1) ����͸� �����Ͽ� �� ��ǻ���� �׷���ī�� �� ���÷��� ������ �޾ƿ´�.
	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(adapter.GetAddressOf());

	// 2) �켱 ���� ������ ������ ��ġ�� �����Ѵ�. ���� �� ���߿� Ŭ������ ��� ������ ������ ���̴�.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	// 3) ����� ������ ���� ���, ��ġ�� ���ؽ�Ʈ�� �����Ѵ�. ������ ���� ����� ����, �÷���, ���� ������ ����Ѵ�.
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

	// 4) ��ġ ���� ���� �� ������ ���� ������ ���, ����� ���� WARP ���� ��ġ ������ �õ��Ѵ�.
	//	  ��ġ ���� ���� �� � ���� �ٷ� ���� �ܰ�� �Ѿ��.

	// NDEBUG: std::assert�� ���� �ش�. �Ϲ������� ������ ��忡�� ���ǳ�, ����� ��忡���� ���ǵ� �� �ִ�.
	//	* ����: �����/������ ���̿��� ���� �ܰ谡 ������, NDEBUG�� assert�� �����ϴ� �ɼǿ� �ش��Ѵ�. ����� ���� ������ ������ ��� NDEBUG�� ������ �Ϻ� �ɼ��� ����.
#if defined(NDEBUG)
	else
	{
		throw std::exception("No Direct3D hardware device found.");
	}
#else // ����� ����� ���
	// �׷���ī�� ���� CPU�� ������ �� �ֵ��� �����Ѵ�. (WARP Device)
	// ����: ����ũ�μ���Ʈ ���� ����Ϸ���, �׷���ī�尡 ���� �� WARP�� �����ϵ��� �����ؾ� �Ѵ�. (������ 7������ Reference�� �ҷ���)
	if (FAILED(hr))
	{
		hr = D3D11CreateDevice(
			nullptr,				// �⺻�� ����� ���
			D3D_DRIVER_TYPE_WARP,	// WARP ����̹� ���
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

#pragma region 5. ����� �ɼ� ����

	// 1) ����� �޽��� ���� �������̽� ������ �������� Ȯ���Ѵ�.
	// 2) �޽����� �ɰ����� ���� ���α׷��� ������Ű�� �ɼ��� �����Ѵ�.
	// 3) ���ʿ��� ����� �޽����� ������� �ʵ��� �����Ѵ�.

#ifndef NDEBUG
	// std::assert�� ������ ����� ����� ��쿡 �����ϴ� ����� �ɼ�.
	// ID3D11Debug�� ID3D11InfoQueue�� ��� ��ȯ �������� Ȯ���Ѵ�.

	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		// ����� �޽����� �����ϴ� �������̽�. ����� �޽����� �������ų�, �����ϰų�, �ܰ躰�� ���͸��� �� �ִ�.
		// ���̷�ƮX�� ���͸� �ɼ��� ���� �ϴ� �뵵�� �������̽��̹Ƿ�, �ɼ� ���� �� �������̽��� �����ص� �ɼ��� �����ȴ�. (ComPtr�̹Ƿ� �� ���� ������ �ڵ� ����)
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			// �޽����� �ɰ����� ���� ���α׷��� ������Ų��. (Break Point�� ������ ��ó�� ����)
			// ���α׷��� �ٿ�ǰų� ���� �߻� �� ������Ű���� �����Ѵ�. ����� ��忡���� ���� �� ���� Ȯ���� �����ϹǷ� ��ó���⸦ ����Ѵ�.
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			// ����� �޽��� �� ���ʿ��� �޽����� ������� �ʵ��� �����Ѵ�.
			// ���⼱ �޽����� ID�� ���͸��ϱ�� �Ѵ�.
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

	// ������ ������ ����̽� �� ���ؽ�Ʈ�� ��� ������ �Ѱ��ְ�, ���� �� ���� ó���Ѵ�.
	ThrowIfFailed(device.As(&m_d3dDevice));
	ThrowIfFailed(context.As(&m_d3dDeviceContext));
	ThrowIfFailed(context.As(&m_d3dAnnotation));	// Annotation�� �׷���ī�尡 �ϴ� �ϵ��� ������ ��� �����Ƿ�, ���ؽ�Ʈ�κ��� �����´�.
}

// �ػ� ���� ���ҽ����� ���� �Ǵ� ������Ѵ�.
void DeviceResources::CreateWindowSizeDependentResources()
{
	// �����찡 ���� ��� ���� ó��.
	if (!m_window)
	{
		throw std::exception("Call SetWindow with a valid Win32 window handle");
	}

#pragma region 1. ������ ����/�� �ʱ�ȭ

	// 1-1) ����̽� ���ؽ�Ʈ�� ���� Ÿ�� �並 null�� ����
	ID3D11RenderTargetView* nullViews[]{ nullptr };
	m_d3dDeviceContext->OMGetRenderTargets(_countof(nullViews), nullViews, nullptr);

	// 1-2) ���� Ÿ�ٰ� ����-���ٽ� ���� �ʱ�ȭ
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_renderTarget.Reset();
	m_depthStencil.Reset();

	// 1-3) DirectX ������ ť�� ���� �۾��� �׷���ī�尡 �Ѳ����� �������� �ϵ��� �Ѵ�.
	//	 �� ������ ����� OMGetRenderTargets �۾��� ť�� ���̹Ƿ�, ���� �۾��� ��� ó���� �� ���������� ���� Ÿ���� ���� �ȴ�.
	m_d3dDeviceContext->Flush();

#pragma endregion

#pragma region 2. �� ���� ũ�� ����

	// 2-1) �߸��� ���� ���� �ʵ��� 1 �̻��� ����� �����Ѵ�. std::min/max<type>�� ����� ��ȯ�Ѵ�.
	const uint backBufferWidth = std::max<uint>(static_cast<uint>(m_outputSize.right - m_outputSize.left), 1u);
	const uint backBufferHeight = std::max<uint>(static_cast<uint>(m_outputSize.bottom - m_outputSize.top), 1u);

	// 2-2) ������10 �̻�(factory4 �̻�)���� �����ϴ� ����ü�� �ɼ� �� �ϳ� �̻��� ���� �ִٸ�, sRGB�� ������� �ʵ��� �����Ѵ�.
	DXGI_FORMAT backBufferFormat = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ?
		NoSRGB(m_backBufferFormat) : m_backBufferFormat;

#pragma endregion

#pragma region 3. ����ü�� ���� �Ǵ� �缳��

	HRESULT hr;

	// 3-1) ����ü���� �̹� �ִ� ���, ����� �缳���Ѵ�.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(
			m_backBufferCount,
			backBufferWidth,
			backBufferHeight,
			backBufferFormat,
			(m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
		);
		
		// DXGI_ERROR_DEVICE_RESET: �׷���ī�尡 ���� ���࿡ �����ϸ�, �ý����� �ٿ�Ǵ� ���� �����ϱ� ���� �׷���ī�带 �ʱ�ȭ�Ѵ�.
		//							�� �� ���� �۾��� �ʱ�ȭ�Ǹ� Reset �޽����� ��ȯ�Ѵ�.
		//							(�ֽ� �׷���ī����� ���������� �̷� ������ ���� ����)
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
#ifdef _DEBUG
			// ����� ����� ��� ������ �˷��ش�.
			// �׷���ī�尡 ���ŵ� ��Ȳ�� �ü���� ������ �� �� �����Ƿ� GetDeviceRemovedReason�� �������� ����Ѵ�.
			// �׷���ī�尡 �� �� ���� ������ ��� ���࿡ ������ Reset�� ���� �ڵ常 ����Ѵ�.
			char buff[64]{};
			sprintf_s(buff, "Device Lost on ResizeBuffers : Reason Code 0x%08X\n",
				static_cast<uint>(hr == DXGI_ERROR_DEVICE_REMOVED) ?
				m_d3dDevice->GetDeviceRemovedReason() : hr
			);

			OutputDebugStringA(buff);
#endif
			// Device Lost ó���� �ϰ� return �Ѵ�.
			OnHandleDeviceLost();
			return;
		}
		else
		{
			// �� ���� �������� ���⼭ ó���� �� ������ ���� ó���� �Ѵ�.
			ThrowIfFailed(hr);
		}
	}
	// 3-2) ����ü���� ���� ���, ���� �����Ѵ�.
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
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;	// ȭ�� ũ�� Ȯ�� �� �ܼ��� �ø��� ����. (ASPECT_RATIO_STRETCH �� ��Ⱦ�� �����ϸ� �ø���. �� ������ ���� �ڽ��� ä���.)
		// FLIP_DISCARD�� �ø� ������� ����ϵ� ��� �� �� ���۴� ����� ���, �Ϲ� DISCARD�� ���� ��ü(flip)�� �ƴ� ����� ����Ѵ�.
		// �ֽ� ���������� �ø��� �����Ѵ�.
		swapChainDesc.SwapEffect = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ? 
			DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
		// ���� ���� ���İ� ��� ���δ� ����.
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = (m_options & c_AllowTearing) ?
			DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

		// Ǯ��ũ�� ���� ������ Description���� �����Ѵ�.
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc{};
		fsSwapChainDesc.Windowed = TRUE;

		// CreateSwapChainForCoreWindow�� UWP(Universal Window)���� �����츦 �����Ѵ�. C#���̾����� C++������ ��밡���ϰ� ����Ǿ���.
		// ���⼭�� ������ ���α׷��� �� C++ ������ ���� CoreWindow�� �ƴ� Hwnd�� ����Ͽ� �����Ѵ�.
		hr = m_dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,			// ���� ���� output���� ȭ���� ������� �ʴ´�. ���⼭�� �������� �ʴ´�.
			m_swapChain.ReleaseAndGetAddressOf()
		);
		ThrowIfFailed(hr);

		// ��ü ȭ�� ��� ��, �׷���ī�� �ڿ��� ������ �����ϰ� �ȴ�. ������ ���� ������ �� ������ ��ȯ �����̰� ����, ��ġ �ս��� ������ �ִ�.
		// ��ü ȭ�� â ��� �� ��ü�� ��� ����ȭ�� ���ҽ��� ������ �Բ� ����ϹǷ� ������ ���� �϶��� �� ������, ��ȯ �����̰� ��������.
		hr = m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER); // Alt + Enter(��ü ȭ��) Ű�� ������ ���´�.
		ThrowIfFailed(hr);
	}

	// 3-3) ���� ������ ������Ʈ�Ѵ�.
	UpdateColorSpace();

#pragma endregion

#pragma region 4. ���� Ÿ�� �� ����

	// 4-1) ���� ü���� ù ��° ���۸� ���� Ÿ�ٿ� �����´�.
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf()));
	ThrowIfFailed(hr);

	// 4-2) Ÿ��(�ؽ���2D)�� ������ ���� Description�� �����ϰ� ���� Ÿ�� �並 �����Ѵ�.
	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
	hr = m_d3dDevice->CreateRenderTargetView(
		m_renderTarget.Get(),
		&renderTargetViewDesc,
		m_renderTargetView.ReleaseAndGetAddressOf()
	);
	ThrowIfFailed(hr);

#pragma endregion

#pragma region 5. ����-���ٽ� ���� ����
	
	// 2D ���ӿ����� �ʿ������, DXGI_FORMAT_UNKNOWN�� ���� ������ �ʴ´�.
	if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
	{
		// 5-1) ����-���ٽ� ���� ����
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

		// 5-2) ����-���ٽ� �� ����
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		hr = m_d3dDevice->CreateDepthStencilView(
			m_depthStencil.Get(),
			&depthStencilViewDesc,
			m_depthStencilView.ReleaseAndGetAddressOf()
		);
		ThrowIfFailed(hr);
	}

#pragma endregion

	// ���������� �� ��Ʈ�� �����Ѵ�.
	m_screenViewport = CD3D11_VIEWPORT(
		0.0f, 0.0f,
		static_cast<float>(backBufferWidth), static_cast<float>(backBufferHeight)
	);
}

// ������ â�� ��� ������ �����Ѵ�. â ũ��� RECT�� ��ȯ�Ͽ� ��� ������ �����Ѵ�.
void DeviceResources::SetWindow(HWND window, int width, int height) noexcept
{
	m_window = window;

	m_outputSize.left = m_outputSize.right = 0;
	m_outputSize.right = width;
	m_outputSize.bottom = height;
}

// ������ ũ�� ���� �� ȣ��ȴ�. �ػ� �� ���÷��̸� �ٲ� ��� ���� ������ ������Ʈ�ϰ� false�� ��ȯ�Ѵ�. ���α׷��� â ũ�Ⱑ ����Ǹ� �ػ� ���� ���ҽ����� ������ϰ� true�� ��ȯ�Ѵ�.
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

// ��� ���� �� ��ġ�� ��� �ʱ�ȭ�ϰ� �ٽ� �����Ѵ�. ���� ���� Device Notify���� Device Lost/Restore�� �����Ѵ�. ����� ����� ��� ��ġ �ʱ�ȭ ���� ��ġ�� ���� ����� ������ ��� ����Ѵ�.
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
			// ReportLiveDeviceObjects: Device�� ���ݱ��� �׿� �ִ� ����� �������� ��� ����Ѵ�. (�÷��׷� ��ü ���/��� ����� ������ �� �ִ�)
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

// ���� ����ȭ ���θ� Ȯ���� �� ���� ü���� Present�� �����Ͽ� ȭ���� ����ϰ�, �� ���۸� ����Ѵ�.
void DeviceResources::Present()
{
	// �ɼ��� ���� ����� ���ο� ���� �����ð��� ����, ����ü���� Present�� ���۽�Ų��.
	HRESULT hr = (m_options & c_AllowTearing) ?
		m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING) : m_swapChain->Present(1, 0);

	// �۵� �� �� ���۴� ������. ����-���ٽ� ���۴� 2D���� �������� �����Ƿ�, �ش� ���۰� ���� ���� ������.
	m_d3dDeviceContext->DiscardView(m_renderTargetView.Get());
	if (m_depthStencilView)
	{
		m_d3dDeviceContext->DiscardView(m_depthStencilView.Get());
	}

	// ��ġ�� ���ų� ���µ� ��� ���� �����ϰ�, ����� ���� ��� �� OnHandleDeviceLost�� ȣ���Ѵ�.
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

	// �� ���� ������ ���� ó���Ѵ�.
	ThrowIfFailed(hr);

	// IsCurrent: DXGI�� ĳ�̵� ��� ������ ��ȿ����(�Ǵ� �����Ǿ�����) Ȯ���Ѵ�. �׷��� ī�带 ������ؾ� �� ��� false�� ��ȯ�Ѵ�.
	//			������ �̷� ������ ���� ���� �ַ� HDR ���� ��� ������� ���� ĳ�� �뷮�� �ʰ��ϴ� ��Ȳ�̴�. �� �ܿ� ���� �߻����� �ʴ´�.
	if (!m_dxgiFactory->IsCurrent())
	{
		CreateFactory();
	}
}

// DXGI ���丮�� �����Ѵ�. ����� ����� ��� �߰��� ����� �ɼ��� �����Ѵ�.
void DeviceResources::CreateFactory()
{
	HRESULT hr;
#if defined(_DEBUG) && (_WIN32_WINNT >= 0x0603)	// 0x0603: ������ 8.1
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

			// 80�� ����: ����Ͱ� 2�� �̻��� ��, �������� ���α׷��� (��� ��� ����Ϳ�) �ٸ� ����ͷ� �̵����� �� �߻��ϴ� ����.
			//			  ����ü���� GetContainingOutput ȣ�� �� ���α׷��� ������ ����Ϳ� ���� ���α׷��� �ִ� ����Ͱ� �ٸ� ��� �߻��Ѵ�. �߿����� ���� ������ �����Ѵ�.
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

// ��ǻ���� �׷���ī�带 �����´�. ���� ������ ���� GPU�� ã�� �� �ִٸ� ã�ƿ���, �ƴ϶�� ù ��° �׷���ī����� Ž���Ͽ� �����´�.
void DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

	// Ư�� ���� �̻��� ���, �� ��ǻ�Ϳ��� ���� ������ ���� GPU�� ã�ƿ� �� �ִ�. (DXGI ���� 1.6 �̻�, ������10 ���彺�� 4 ������Ʈ �̻󿡼� ����)
#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = m_dxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(	// �׷���ī�带 �ϳ��� �����´�. �� �̻� ������ �׷���ī�尡 ���� ������ �����Ѵ�.
				adapterIndex,							// ������ �׷���ī���� �ε���
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,	// �ε��� ���� ����. ���⼱ ������ GPU�� ����(= ���� �ε�������) ������ �����Ѵ�.
				IID_PPV_ARGS(adapter.GetAddressOf())	// ������ �׷���ī�� ������ �����ϱ� ���� �������̽�
			));
			adapterIndex++)
		{
			// �ش� �׷���ī���� ������ �о, ����Ʈ������ ��쿡�� �ٸ� �׷���ī�带 Ž���Ѵ�.
			// * ����Ʈ���� �÷��״� ������ �⺻ ����̹�(Microsoft Basic Render Driver)���� ��ȯ�ȴ�.
			DXGI_ADAPTER_DESC1 desc;
			ThrowIfFailed(adapter->GetDesc1(&desc));
			
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// ����� ����� ��� � �׷���ī�带 ����ϴ��� ����Ѵ�.
#ifdef _DEBUG
			wchar_t buff[256]{};
			// %04x �Ǵ� %04X : 16���� 4�ڸ� ��� (A~F�� ��� x�� �ҹ��ڷ�, X�� �빮�ڷ�)
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif
			// ������ �׷���ī�� ��뿡 ������ ���ٸ� break
			break;
		}
	}
#endif

	// ���� if�� ���Կ� �����ϰų� �׷���ī�� Ž���� ������ ���, �� ���� �׷���ī����� �ϳ��� Ž���Ѵ�.
	// Ž���� �׷���ī�忡 ���ؼ��� ���� ������ ó��.
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

	// unique/shared ptr�� ������ ������ std::move()�� ����ؼ� �̵���Ű����, ComPtr�� .Detach()�� �̿��Ѵ�.
	// �ܼ��� ������ ��� adapter�� �� ���� ������ ���� ī��Ʈ�� 0�� �Ǿ� �������� �޸𸮰� �����ǹǷ� ����.
	*ppAdapter = adapter.Detach();
}

// ���α׷��� ��µǴ� ����� ������ ã�Ƽ� �˸��� ���� ������ �����ϰ�, ���� ü���� ������Ʈ�Ѵ�.
void DeviceResources::UpdateColorSpace()
{
	// HDTV�� ǥ�� ����
	DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

	bool isDisplayHDR10 = false;

	// HDR10 ���� ���� Ȯ��. ������10 ���彺��2 ������Ʈ���� �����Ѵ�.
#if defined(NTDDI_WIN10_RS2)
	// ���α׷��� ��µǴ� ����ʹ�, ���α׷��� �� ���� ���� �ִ� ����͸� Ÿ������ �Ѵ�.
	// �̸� �Ǵ��ϱ� ���ؼ� ���� ü���� �ʼ������� �־�� �Ѵ�.
	if (m_swapChain)
	{
		ComPtr<IDXGIOutput> output;
		// GetContainingOutput : ���α׷��� ���� ���� ���� �ִ� ����͸� output�� �����´�.
		if (SUCCEEDED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
		{
			ComPtr<IDXGIOutput6> output6;
			if (SUCCEEDED(output.As(&output6)))
			{
				DXGI_OUTPUT_DESC1 desc;
				ThrowIfFailed(output6->GetDesc1(&desc));

				// �ش� ������� ���� ������ HDR10 ���� ��� isDisplayHDR10�� true�� �ٲ��ش�. UHD(4K, 8K) ����Ϳ��� �����Ѵ�.
				if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
				{
					isDisplayHDR10 = true;
				}
			}
		}
	}
#endif

	// HDR �ɼ��� ���� �ְ�, �� ����Ϳ��� HDR10�� �����Ѵٸ� ���� ������ ��� �̹���(�� ���� ����)�� �°� �����Ѵ�.
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
		// CheckColorSpaceSupport: ���� ����(colorSpace)�� �ش� ����ü�ο��� �����Ǵ��� Ȯ���Ͽ�, �ش� ������ colorSpaceSupport�� �÷��׷� �����Ѵ�.
		if (SUCCEEDED(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)) &&
			(colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		{
			ThrowIfFailed(swapChain3->SetColorSpace1(colorSpace));
		}
	}
}
