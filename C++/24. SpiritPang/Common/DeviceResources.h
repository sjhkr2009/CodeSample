#pragma once

// DeviceResources: ��ġ�� ȭ�� ���۸� �����մϴ�. DirectX�� �ʱ�ȭ�� ����, ���ҽ��� ������ ������� ����մϴ�.
// * DirectX TK�� �������� �ۼ��Ǿ����ϴ�.

namespace DX
{
	// IDeviceNotify: ��ġ ���� ���ҽ��� �ִ� Ŭ�������� ����մϴ�.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

		// �������̽��� ���ο��� �޸� ������ ���� �ʴ´�.
		// �޸� ������ ����ϴ� �Ҹ��ڸ� default�� ��������ν� �̸� ǥ���Ѵ�. �Ҹ��ڴ� �ڵ� ���� ������ �����Ǵµ�, delete �ع����� �� �� ������ ���� ����.
		// (��� ������ �����ϳ�, �ٸ� ���α׷��Ӱ� �� �������̽��� �ڽ� Ŭ�������� �Ҹ��ڸ� ȣ������ �ʵ��� �ϱ� ���� ǥ��)
	protected:
		~IDeviceNotify() = default;
	};
	
	class DeviceResources
	{
	private:
		Microsoft::WRL::ComPtr<IDXGIFactory2>				m_dxgiFactory;
		Microsoft::WRL::ComPtr<ID3D11Device1>				m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1>		m_d3dDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>				m_swapChain;
		Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>	m_d3dAnnotation;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_renderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;

		D3D11_VIEWPORT m_screenViewport;
		DXGI_FORMAT m_backBufferFormat;
		DXGI_FORMAT m_depthBufferFormat;
		UINT m_backBufferCount;
		D3D_FEATURE_LEVEL m_d3dMinFeatureLevel;
		D3D_FEATURE_LEVEL m_d3dFeatureLevel;

		HWND m_window;
		RECT m_outputSize;
		
		DXGI_COLOR_SPACE_TYPE m_colorSpace;
		uint m_options;

		IDeviceNotify* m_deviceNotify;

	public:
		// ����ü�� �ɼ� (��Ʈ �÷���)
		static const unsigned int c_FlipPresent = 0x1;
		static const unsigned int c_AllowTearing = 0x2;
		static const unsigned int c_EnableHDR = 0x4;

	private:
		void CreateFactory();
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
		void UpdateColorSpace();

	public:
		DeviceResources(
			DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
			DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
			UINT backBufferCount = 2,
			D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_10_0,
			unsigned int flags = c_FlipPresent
		) noexcept;

		// �Ҹ��ڿ��� �޸� ������ ���� �����Ƿ�, �Ҹ��ڿ��� �ƹ��͵� ���� ������ ����Ѵ�.
		// ���� �޸� ������ �ʿ��ϸ� ������ Release �Լ��� ����� ������ ����.
		~DeviceResources() = default;

		// �� Ŭ������ �ϳ��� �����ؾ� �ϹǷ� ���� �����ڿ� �����ڴ� �����Ѵ�. (�̱��� ��� �� ��Ƽ������ ȯ�濡 �����)
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator =(const DeviceResources&) = delete;

		// �̵� �����ڿ� �̵� ���� �����ڴ� �⺻������ ����Ѵ�.
		DeviceResources(DeviceResources&&) = default;
		DeviceResources& operator =(DeviceResources&&) = default;

		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void SetWindow(HWND window, int width, int height) noexcept;
		bool OnWindowSizeChanged(int width, int height);
		void OnHandleDeviceLost();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotifier) noexcept
		{
			m_deviceNotify = deviceNotifier;
		}
		void Present();

	public:
		RECT					GetOutputSize() const noexcept { return m_outputSize; }
		auto					GetD3DDevice() const noexcept { return m_d3dDevice.Get(); }
		auto					GetD3DDeviceContext() const noexcept { return m_d3dDeviceContext.Get(); }
		auto					GetSwapChain() const noexcept { return m_swapChain.Get(); }
		auto					GetDXGIFactory() const noexcept { return m_dxgiFactory.Get(); }
		HWND					GetWindow() const noexcept { return m_window; }

		D3D_FEATURE_LEVEL		GetDeviceFeatureLevel() const noexcept { return m_d3dFeatureLevel; }
		ID3D11Texture2D*		GetRenderTarget() const noexcept { return m_renderTarget.Get(); }
		ID3D11Texture2D*		GetDepthStencil() const noexcept { return m_depthStencil.Get(); }
		ID3D11RenderTargetView* GetRenderTargetView() const noexcept { return m_renderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencilView() const noexcept { return m_depthStencilView.Get(); }

		DXGI_FORMAT				GetBackBufferFormat() const noexcept { return m_backBufferFormat; }
		DXGI_FORMAT				GetDepthBufferFormat() const noexcept { return m_depthBufferFormat; }
		D3D11_VIEWPORT			GetScreenViewport() const noexcept { return m_screenViewport; }
		UINT					GetBackBufferCount() const noexcept { return m_backBufferCount; }
		DXGI_COLOR_SPACE_TYPE	GetColorSpace() const noexcept { return m_colorSpace; }
		unsigned int			GetDeviceOptions() const noexcept { return m_options; }

	public:
		// �׷��� ����� ���� PIXBeginEvent ~ PIXEndEvent ���̿� ����� ������ ������ �Է��� �̸����� ǥ��˴ϴ�.
		void PIXBeginEvent(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotation->BeginEvent(name);
		}
		// �׷��� ����� ���� PIXBeginEvent ~ PIXEndEvent ���̿� ����� ������ ������ �Է��� �̸����� ǥ��˴ϴ�.
		void PIXEndEvent()
		{
			m_d3dAnnotation->EndEvent();
		}
		// DirectX�� ����ϴ� ����� ���� �ܿ�, �׷��� ����� ���� ����ڰ� ���ϴ� �α׸� ���� �� �ֽ��ϴ�. PIXBeginEvent ~ PIXEndEvent ���̿����� �۵��մϴ�.
		void PIXSetMarker(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotation->SetMarker(name);
		}
	};
}

