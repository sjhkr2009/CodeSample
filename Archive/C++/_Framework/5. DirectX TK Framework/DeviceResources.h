#pragma once

// DeviceResources: 장치와 화면 버퍼를 관리합니다. DirectX의 초기화와 해제, 리소스의 해제와 재생성을 담당합니다.
// * DirectX TK를 바탕으로 작성되었습니다.

namespace DX
{
	// IDeviceNotify: 장치 종속 리소스가 있는 클래스에서 상속합니다.
	interface IDeviceNotify
	{
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

		// 인터페이스는 내부에서 메모리 관리를 하지 않는다.
		// 메모리 해제를 담당하는 소멸자를 default로 명시함으로써 이를 표기한다. 소멸자는 코드 블럭이 끝나면 삭제되는데, delete 해버리면 이 때 에러가 나니 주의.
		// (없어도 동작은 동일하나, 다른 프로그래머가 이 인터페이스의 자식 클래스에서 소멸자를 호출하지 않도록 하기 위해 표기)
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
		// 스왑체인 옵션 (비트 플래그)
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

		// 소멸자에서 메모리 해제를 하지 않으므로, 소멸자에선 아무것도 하지 않음을 명시한다.
		// 추후 메모리 해제가 필요하면 별도로 Release 함수를 만들어 구현할 예정.
		~DeviceResources() = default;

		// 이 클래스는 하나만 존재해야 하므로 복사 생성자와 연산자는 삭제한다. (싱글톤 사용 시 멀티스레드 환경에 취약함)
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator =(const DeviceResources&) = delete;

		// 이동 생성자와 이동 대입 연산자는 기본값으로 사용한다.
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
		// 그래픽 디버깅 툴에 PIXBeginEvent ~ PIXEndEvent 사이에 수행된 동작의 정보가 입력한 이름으로 표기됩니다.
		void PIXBeginEvent(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotation->BeginEvent(name);
		}
		// 그래픽 디버깅 툴에 PIXBeginEvent ~ PIXEndEvent 사이에 수행된 동작의 정보가 입력한 이름으로 표기됩니다.
		void PIXEndEvent()
		{
			m_d3dAnnotation->EndEvent();
		}
		// DirectX가 기록하는 디버깅 정보 외에, 그래픽 디버깅 툴에 사용자가 원하는 로그를 남길 수 있습니다. PIXBeginEvent ~ PIXEndEvent 사이에서만 작동합니다.
		void PIXSetMarker(_In_z_ const wchar_t* name)
		{
			m_d3dAnnotation->SetMarker(name);
		}
	};
}

