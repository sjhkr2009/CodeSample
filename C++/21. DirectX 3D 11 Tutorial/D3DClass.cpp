#include "D3DClass.h"

D3DClass::D3DClass() :
	mSwapChain{ 0 },
	mDevice{ 0 },
	mDeviceContext{ 0 },
	mRenderTargetView{ 0 },
	mDepthStencilbuffer{ 0 },
	mDepthStencilState{ 0 },
	mDepthStencilView{ 0 },
	mRasterState{ 0 }
{
}

bool D3DClass::Init(int screenWidth, int screenHeight,
	bool vsync, HWND hwnd, bool fullscreen, 
	float screenDepth, float screenNear)
{
	HRESULT hr;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, numerator, denominator;
	unsigned int stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	mVsyncEnabled = vsync;

#pragma region 1. 주사율 정보 받아오기

	// 1. 주사율 정보 받아오기
	// Direct3D 초기화 전에 그래픽카드와 모니터에서 주사율(Refresh Rate) 정보를 받아와야 한다.
	// 이 작업을 수행하지 않고 기본값을 사용하면, 버퍼 플립 대신 비트 블릿(Bit BLT) 방식을 사용하여 성능저하와 오류를 일으킨다.

	// 1-1. 다이렉트X 그래픽 인터페이스(DXGI) 팩토리 생성
	hr = CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		(void**)&factory
	);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-2. 팩토리를 이용하여 주 그래픽 인터페이스(그래픽카드)를 위한 어댑터를 만든다.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-3. 주 어댑터 출력(모니터)을 나열한다.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-4. 디스플레이 포맷(RGBA 32비트)에 적합한 모니터의 개수를 확인한다.
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		NULL
	);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-5. 이 모니터 및 그래픽카드 조합에서 사용 사능한 모든 디스플레이 모드의 리스트를 생성한다.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 1-6. 디스플레이 모드 구조체를 채운다.
	hr = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes,
		displayModeList
	);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-7. 이제 디스플레이 모드들 중에서 스크린 너비와 높이에 맞는 모드를 찾는다. 찾으면 해당 모니터에 대한 주사율을 저장한다.
	for (int i = 0; i < numModes; i++)
	{
		if ((displayModeList[i].Width == (unsigned int)screenWidth) ||
			(displayModeList[i].Height == (unsigned int)screenHeight))
		{
			// 주사율 값의 분자와 분모를 저장한다.
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}
#pragma endregion

#pragma region 2. 그래픽카드 정보 받아오기

	// 2. 그래픽카드 정보 받아오기

	// 2-1. 어댑터(그래픽카드)의 설명을 받아온다.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		return false;
	}

	// 2-2. 그래픽카드의 메모리를 MB 단위로 받아온다.
	//	ㄴ adapterDesc.DedicatedVideoMemory는 바이트 단위로 반환하므로 1024*1024로 나눠 MB 단위로 변환한다.
	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 2-3. 그래픽카드 이름을 문자로 변환하여 저장한다.
	// TODO: stringLength을 unsigned long long으로 선언하니 에러 발생하여 unsigned int로 변경함.
	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

#pragma endregion

#pragma region 3. 앞에서 사용한 인터페이스 해제

	// 3. 인터페이스 해제
	// 주사율 및 그래픽카드 정보를 저장했으니, 이에 사용된 요소들은 해제한다.

	// 3-1. 디스플레이 모드 목록 해제
	delete[] displayModeList;
	displayModeList = nullptr;

	// 3-2. 어댑터 출력 및 어댑터 해제
	adapterOutput->Release();
	adapter->Release();
	adapterOutput = nullptr;
	adapter = nullptr;

	// 3-3. 팩토리 해제
	factory->Release();
	factory = nullptr;

#pragma endregion

#pragma region 4. 다이렉트X 초기화 1 - 스왑 체인 설정

	// 4. 스왑 체인 초기화
	//	프론트 버퍼와 하나 이상의 백 버퍼를 사용, 백 버퍼에 그려질 내용을 그린 후 프론트 버퍼로 교체하는 스왑 체인을 초기화한다.
	
	// 4-1. 구조체 메모리 초기화
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 4-2. 백 버퍼의 개수를 설정하고, 크기와 포맷을 지정한다.
	//	여기서는 1개의 백 버퍼와 32비트의 RGBA 색상 포맷을 사용하기로 한다.
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 4-3. 백 버퍼의 주사율(분자/분모) 설정
	//	수직 동기화(Vertical Synchronization) 여부에 따라, 주사율과 맞추거나 무제한(대기시간 0)으로 설정한다.
	swapChainDesc.BufferDesc.RefreshRate.Numerator =
		mVsyncEnabled ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator =
		mVsyncEnabled ? denominator : 1;

	// 4-4. 버퍼의 용도를 설정한다.
	// 셰이더 입력용(SHADER_INPUT), 렌더 타겟 출력(RENDER_TARGET_OUTPUT), 단순 백 버퍼(BACK_BUFFER)나 읽기 전용(READ_ONLY) 등의 옵션이 있다.
	// 여기서는 출력 용도로 사용한다.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 4-5. 스왑 체인이 렌더링할 창과 전체 화면 여부를 지정한다.
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = fullscreen ? false : true;

	// 4-6. 멀티 샘플링 설정. 추출할 샘플 개수와 품질 수준을 입력한다.
	//	여기서는 사용하지 않을 것이므로 1과 0 입력.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 4-7. 화면 그리기 방법(스캔 라인), 화면과 해상도 불일치 시 처리(확대) 방식 설정
	//	여기서는 둘 다 지정하지 않는다.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4-8. 출력 후 백 버퍼를 어떻게 처리할지 지정한다.
	//	여기서는 출력 후 폐기한다. (버퍼 유지 시 비용이 많이 든다)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 4-9. 스왑 체인 동작에 대한 플래그 지정. (DXGI_SWAP_CHAIN_FLAG)
	//	여기서는 별도의 플래그를 지정하지 않는다. 참고로 디스플레이 모드 변경 허용 플래그는 DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	swapChainDesc.Flags = 0;

#pragma endregion

#pragma region 5. 다이렉트X 초기화 2 - 장치 및 스왑체인 생성

	// 5. DirectX 11에서 사용할 장치와 스왑 체인을 생성한다.
	//	DirectX 11을 지원하는 장치(그래픽카드)가 필요하다. 없을 경우 두 번째 변수를 D3D_DRIVER_TYPE_REFERENCE로 지정하여 그래픽카드 대신 CPU를 사용할 수 있다.

	// feature level(기능 수준, 버전) 설정. NULL이면 기본값을 사용하며, 버전 개수도 0개로 입력하면 된다.
	// 여기선 NULL 대신 DirectX 11.0 버전을 사용하도록 한다.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel, 1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mSwapChain,
		&mDevice,
		NULL,
		&mDeviceContext
	);

	if (FAILED(hr))
	{
		return false;
	}

#pragma endregion

#pragma region 6. 다이렉트X 초기화 3 - 렌더 타겟 설정

	// 6. 스왑 체인의 백 버퍼를 렌더 타겟으로 지정하여, 렌더 타겟을 생성한다.

	// 6-1. 스왑 체인으로부터 백 버퍼 포인터 가져오기
	//	두 번째 인자는 IID_ID3D11RenderTargetView로 입력할 수 있다.
	//	또는 ID3D11Texture2D* 타입의 변수를 선언해두고, IID_PPV_ARGS(해당 변수 주소) 로 2,3번째 인자를 대체할 수 있다.
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
	if (FAILED(hr))
	{
		return false;
	}

	// 6-2. 백 버퍼 포인터를 통해 렌더 타겟 뷰 생성
	hr = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}

	// 6-3. 뷰 생성 후에는 스왑 체인의 백 버퍼가 렌더 타겟으로 지정되었으니, 연결용으로 쓴 백 버퍼 포인터는 해제한다.
	backBufferPtr->Release();
	backBufferPtr = nullptr;

#pragma endregion

#pragma region 7. 다이렉트X 초기화 4 - 깊이-스텐실 버퍼 설정

	// 7. 깊이 버퍼와 스텐실 버퍼에 대한 설명을 작성한다.
	//	참고로 깊이 버퍼와 스텐실 버퍼는 용량이 크지 않으므로, 하나의 자료형에 비트를 나누어 저장하는 것이 효율적이다.
	//	D3D11_DEPTH_STENCIL_VIEW_DESC 구조체로 작성되나, C를 붙이면 클래스화되어 생성자를 통해 생성할 수 있다.
	//	여기서는 구조체로 생성한다.

	// 7-1. 구조체 메모리를 초기화하고, 깊이-스텐실 버퍼에 대한 설명을 작성한다.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable		= true;							// 깊이 비트 활성화
	depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;	// 쓰기 용도
	depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;		// 깊이가 작은(=앞쪽에 있는) 오브젝트를 렌더링
	depthStencilDesc.StencilEnable		= true;							// 스텐실 비트 활성화
	depthStencilDesc.StencilReadMask	= 0xFF;							// 모두 읽기 (마스크 비트 전부 1로 채움)
	depthStencilDesc.StencilWriteMask	= 0xFF;							// 모두 쓰기

	// 픽셀이 전면을 향할 때
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// 픽셀이 후면을 향할 때
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// 7-2. 해당 설명을 이용하여 깊이-스텐실 상태를 만든다.
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	// 7-3. 디바이스 컨텍스트를 이용하여 깊이-스텐실 상태를 설정한다.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

#pragma endregion

#pragma region 8. 다이렉트X 초기화 5 - 깊이-스텐실 뷰 생성 및 백 버퍼와 합치기

	// 8. 깊이-스텐실 버퍼의 데이터가 저장될 공간을 텍스쳐로 생성하고, View를 만든다.

	// 8-1. 구조체 메모리를 초기화하고, 깊이 버퍼에 대한 설명을 작성한다.
	//	D3D11_TEXTURE2D_DESC 구조체로 작성되나, C를 붙일 경우 클래스로 생성할 수 있다. (생성자를 통해 생성 가능)
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;	// Mipmap 생성 개수. 0으로 설정 시 가능한 모든 크기로 생성한다.
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 8-2. 해당 설명을 이용하여 깊이-스텐실 버퍼가 작성될 텍스쳐2D를 만든다.
	hr = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilbuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// 8-3. 구조체 메모리를 초기화하고, 깊이-스텐실 뷰에 대한 설명을 작성한다.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 8-4. 생성된 텍스쳐 공간과 설정을 이용하여, 깊이-스텐실 뷰를 생성한다.
	hr = mDevice->CreateDepthStencilView(mDepthStencilbuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// 8-5. 생성된 깊이-스텐실 버퍼를 백 버퍼(렌더 타겟 뷰)와 합친다(Bind).
	//	렌더링 파이프라인의 Output Merger Stage 단계에서 작동하므로 함수에 OM이 붙는다.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

#pragma endregion

#pragma region 9. 다이렉트 X 초기화 6 - 뷰 포트 설정

	// 9. 뷰 포트를 화면 크기로 설정한다.
	//	3D -> 2D 투영 과정에서 쓰이므로 래스터라이저에서 사용된다.

	// 9-1. 뷰 포트 구조체 설정
	//	D3D11_VIEWPORT 구조체를 사용하지만, CD3D11_VIEWPORT로 클래스로 대체 가능하다. (ZeroMemory는?)
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	// 9-2. 뷰 포트 생성. 래스터라이저의 약자인 RS가 붙는다.
	mDeviceContext->RSSetViewports(1, &viewport);

#pragma endregion

#pragma region 10. 래스터라이저 상태 생성 및 설정

	// 10. 래스터라이저 상태를 설정한다.
	//	다각형의 어떤 면을 그릴 지, 와이어 프레임 모드로 렌더링, 컬링 방식 등 폴리곤이 렌더링되는 방식을 제어할 수 있다.
	//	기본적으로 설정되어 있으나, 값을 변경하려면 직접 래스터라이저 상태를 만들어야 한다. 여기선 기본값과 동일하게 만들어보기로 한다.
	
	// 10-1. 래스터라이저 상태 구조체 작성 (마찬가지로 C를 붙여 클래스로 생성 가능)
	rasterDesc.AntialiasedLineEnable	= false;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.f;
	rasterDesc.DepthClipEnable			= true;
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise	= false;
	rasterDesc.MultisampleEnable		= false;
	rasterDesc.ScissorEnable			= false;
	rasterDesc.SlopeScaledDepthBias		= 0.f;

	// 10-2. 래스터라이저 상태 생성
	hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(hr))
	{
		return false;
	}

	// 10-3. 래스터라이저 상태 설정
	mDeviceContext->RSSetState(mRasterState);

#pragma endregion

#pragma region 11. 행렬(Matrix) 생성

	// 11. 렌더링 파이프라인에서 사용되는 매트릭스들을 생성한다.
	//	일반적으로 View Matrix, World Matrix, Projection Matrix, Orthographic Matrix가 있다.
	//	렌더링에 사용될 셰이더에 전달될 수 있도록 사본을 보관하는 용도로 이 클래스에 저장되며, XMMatrix 계열의 함수로 만든다.
	//	우리가 보는 장면의 위치를 계산하는 View Matrix는 추후 카메라에서 처리하고, 나머지만 만들기로 한다.

	// 11-1. 월드 좌표계 (World Matrix) 초기화
	//	오브젝트의 정점 변환이나 Transform 조정에 사용된다.
	mWorldMatrix = XMMatrixIdentity();

	// 11-2. 투영 행렬 (Projection Matrix) 설정 및 생성
	//	3D 장면을 2D 뷰포트 공간으로 변환하는 데 사용된다.
	fieldOfView = 3.1415926f / 4.0f;
	screenAspect = (float)screenWidth / screenHeight;

	mProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// 11-3. 직교 투영 행렬 (Orthographic Matrix) 생성
	//	UI같은 2D 그래픽 요소와 글꼴 등의 렌더링에 쓰인다.
	mOrthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

#pragma endregion
	
	return true;
}

