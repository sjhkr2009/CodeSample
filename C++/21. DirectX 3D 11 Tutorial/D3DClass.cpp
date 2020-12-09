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

#pragma region 1. �ֻ��� ���� �޾ƿ���

	// 1. �ֻ��� ���� �޾ƿ���
	// Direct3D �ʱ�ȭ ���� �׷���ī��� ����Ϳ��� �ֻ���(Refresh Rate) ������ �޾ƿ;� �Ѵ�.
	// �� �۾��� �������� �ʰ� �⺻���� ����ϸ�, ���� �ø� ��� ��Ʈ ��(Bit BLT) ����� ����Ͽ� �������Ͽ� ������ ����Ų��.

	// 1-1. ���̷�ƮX �׷��� �������̽�(DXGI) ���丮 ����
	hr = CreateDXGIFactory(
		__uuidof(IDXGIFactory),
		(void**)&factory
	);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-2. ���丮�� �̿��Ͽ� �� �׷��� �������̽�(�׷���ī��)�� ���� ����͸� �����.
	hr = factory->EnumAdapters(0, &adapter);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-3. �� ����� ���(�����)�� �����Ѵ�.
	hr = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(hr))
	{
		return false;
	}

	// 1-4. ���÷��� ����(RGBA 32��Ʈ)�� ������ ������� ������ Ȯ���Ѵ�.
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

	// 1-5. �� ����� �� �׷���ī�� ���տ��� ��� ����� ��� ���÷��� ����� ����Ʈ�� �����Ѵ�.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 1-6. ���÷��� ��� ����ü�� ä���.
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

	// 1-7. ���� ���÷��� ���� �߿��� ��ũ�� �ʺ�� ���̿� �´� ��带 ã�´�. ã���� �ش� ����Ϳ� ���� �ֻ����� �����Ѵ�.
	for (int i = 0; i < numModes; i++)
	{
		if ((displayModeList[i].Width == (unsigned int)screenWidth) ||
			(displayModeList[i].Height == (unsigned int)screenHeight))
		{
			// �ֻ��� ���� ���ڿ� �и� �����Ѵ�.
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}
#pragma endregion

#pragma region 2. �׷���ī�� ���� �޾ƿ���

	// 2. �׷���ī�� ���� �޾ƿ���

	// 2-1. �����(�׷���ī��)�� ������ �޾ƿ´�.
	hr = adapter->GetDesc(&adapterDesc);
	if (FAILED(hr))
	{
		return false;
	}

	// 2-2. �׷���ī���� �޸𸮸� MB ������ �޾ƿ´�.
	//	�� adapterDesc.DedicatedVideoMemory�� ����Ʈ ������ ��ȯ�ϹǷ� 1024*1024�� ���� MB ������ ��ȯ�Ѵ�.
	mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 2-3. �׷���ī�� �̸��� ���ڷ� ��ȯ�Ͽ� �����Ѵ�.
	// TODO: stringLength�� unsigned long long���� �����ϴ� ���� �߻��Ͽ� unsigned int�� ������.
	error = wcstombs_s(&stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

#pragma endregion

#pragma region 3. �տ��� ����� �������̽� ����

	// 3. �������̽� ����
	// �ֻ��� �� �׷���ī�� ������ ����������, �̿� ���� ��ҵ��� �����Ѵ�.

	// 3-1. ���÷��� ��� ��� ����
	delete[] displayModeList;
	displayModeList = nullptr;

	// 3-2. ����� ��� �� ����� ����
	adapterOutput->Release();
	adapter->Release();
	adapterOutput = nullptr;
	adapter = nullptr;

	// 3-3. ���丮 ����
	factory->Release();
	factory = nullptr;

#pragma endregion

#pragma region 4. ���̷�ƮX �ʱ�ȭ 1 - ���� ü�� ����

	// 4. ���� ü�� �ʱ�ȭ
	//	����Ʈ ���ۿ� �ϳ� �̻��� �� ���۸� ���, �� ���ۿ� �׷��� ������ �׸� �� ����Ʈ ���۷� ��ü�ϴ� ���� ü���� �ʱ�ȭ�Ѵ�.
	
	// 4-1. ����ü �޸� �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 4-2. �� ������ ������ �����ϰ�, ũ��� ������ �����Ѵ�.
	//	���⼭�� 1���� �� ���ۿ� 32��Ʈ�� RGBA ���� ������ ����ϱ�� �Ѵ�.
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 4-3. �� ������ �ֻ���(����/�и�) ����
	//	���� ����ȭ(Vertical Synchronization) ���ο� ����, �ֻ����� ���߰ų� ������(���ð� 0)���� �����Ѵ�.
	swapChainDesc.BufferDesc.RefreshRate.Numerator =
		mVsyncEnabled ? numerator : 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator =
		mVsyncEnabled ? denominator : 1;

	// 4-4. ������ �뵵�� �����Ѵ�.
	// ���̴� �Է¿�(SHADER_INPUT), ���� Ÿ�� ���(RENDER_TARGET_OUTPUT), �ܼ� �� ����(BACK_BUFFER)�� �б� ����(READ_ONLY) ���� �ɼ��� �ִ�.
	// ���⼭�� ��� �뵵�� ����Ѵ�.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 4-5. ���� ü���� �������� â�� ��ü ȭ�� ���θ� �����Ѵ�.
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = fullscreen ? false : true;

	// 4-6. ��Ƽ ���ø� ����. ������ ���� ������ ǰ�� ������ �Է��Ѵ�.
	//	���⼭�� ������� ���� ���̹Ƿ� 1�� 0 �Է�.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 4-7. ȭ�� �׸��� ���(��ĵ ����), ȭ��� �ػ� ����ġ �� ó��(Ȯ��) ��� ����
	//	���⼭�� �� �� �������� �ʴ´�.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 4-8. ��� �� �� ���۸� ��� ó������ �����Ѵ�.
	//	���⼭�� ��� �� ����Ѵ�. (���� ���� �� ����� ���� ���)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 4-9. ���� ü�� ���ۿ� ���� �÷��� ����. (DXGI_SWAP_CHAIN_FLAG)
	//	���⼭�� ������ �÷��׸� �������� �ʴ´�. ����� ���÷��� ��� ���� ��� �÷��״� DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	swapChainDesc.Flags = 0;

#pragma endregion

#pragma region 5. ���̷�ƮX �ʱ�ȭ 2 - ��ġ �� ����ü�� ����

	// 5. DirectX 11���� ����� ��ġ�� ���� ü���� �����Ѵ�.
	//	DirectX 11�� �����ϴ� ��ġ(�׷���ī��)�� �ʿ��ϴ�. ���� ��� �� ��° ������ D3D_DRIVER_TYPE_REFERENCE�� �����Ͽ� �׷���ī�� ��� CPU�� ����� �� �ִ�.

	// feature level(��� ����, ����) ����. NULL�̸� �⺻���� ����ϸ�, ���� ������ 0���� �Է��ϸ� �ȴ�.
	// ���⼱ NULL ��� DirectX 11.0 ������ ����ϵ��� �Ѵ�.
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

#pragma region 6. ���̷�ƮX �ʱ�ȭ 3 - ���� Ÿ�� ����

	// 6. ���� ü���� �� ���۸� ���� Ÿ������ �����Ͽ�, ���� Ÿ���� �����Ѵ�.

	// 6-1. ���� ü�����κ��� �� ���� ������ ��������
	//	�� ��° ���ڴ� IID_ID3D11RenderTargetView�� �Է��� �� �ִ�.
	//	�Ǵ� ID3D11Texture2D* Ÿ���� ������ �����صΰ�, IID_PPV_ARGS(�ش� ���� �ּ�) �� 2,3��° ���ڸ� ��ü�� �� �ִ�.
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
	if (FAILED(hr))
	{
		return false;
	}

	// 6-2. �� ���� �����͸� ���� ���� Ÿ�� �� ����
	hr = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}

	// 6-3. �� ���� �Ŀ��� ���� ü���� �� ���۰� ���� Ÿ������ �����Ǿ�����, ��������� �� �� ���� �����ʹ� �����Ѵ�.
	backBufferPtr->Release();
	backBufferPtr = nullptr;

#pragma endregion

#pragma region 7. ���̷�ƮX �ʱ�ȭ 4 - ����-���ٽ� ���� ����

	// 7. ���� ���ۿ� ���ٽ� ���ۿ� ���� ������ �ۼ��Ѵ�.
	//	����� ���� ���ۿ� ���ٽ� ���۴� �뷮�� ũ�� �����Ƿ�, �ϳ��� �ڷ����� ��Ʈ�� ������ �����ϴ� ���� ȿ�����̴�.
	//	D3D11_DEPTH_STENCIL_VIEW_DESC ����ü�� �ۼ��ǳ�, C�� ���̸� Ŭ����ȭ�Ǿ� �����ڸ� ���� ������ �� �ִ�.
	//	���⼭�� ����ü�� �����Ѵ�.

	// 7-1. ����ü �޸𸮸� �ʱ�ȭ�ϰ�, ����-���ٽ� ���ۿ� ���� ������ �ۼ��Ѵ�.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable		= true;							// ���� ��Ʈ Ȱ��ȭ
	depthStencilDesc.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;	// ���� �뵵
	depthStencilDesc.DepthFunc			= D3D11_COMPARISON_LESS;		// ���̰� ����(=���ʿ� �ִ�) ������Ʈ�� ������
	depthStencilDesc.StencilEnable		= true;							// ���ٽ� ��Ʈ Ȱ��ȭ
	depthStencilDesc.StencilReadMask	= 0xFF;							// ��� �б� (����ũ ��Ʈ ���� 1�� ä��)
	depthStencilDesc.StencilWriteMask	= 0xFF;							// ��� ����

	// �ȼ��� ������ ���� ��
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// �ȼ��� �ĸ��� ���� ��
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// 7-2. �ش� ������ �̿��Ͽ� ����-���ٽ� ���¸� �����.
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		return false;
	}

	// 7-3. ����̽� ���ؽ�Ʈ�� �̿��Ͽ� ����-���ٽ� ���¸� �����Ѵ�.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

#pragma endregion

#pragma region 8. ���̷�ƮX �ʱ�ȭ 5 - ����-���ٽ� �� ���� �� �� ���ۿ� ��ġ��

	// 8. ����-���ٽ� ������ �����Ͱ� ����� ������ �ؽ��ķ� �����ϰ�, View�� �����.

	// 8-1. ����ü �޸𸮸� �ʱ�ȭ�ϰ�, ���� ���ۿ� ���� ������ �ۼ��Ѵ�.
	//	D3D11_TEXTURE2D_DESC ����ü�� �ۼ��ǳ�, C�� ���� ��� Ŭ������ ������ �� �ִ�. (�����ڸ� ���� ���� ����)
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;	// Mipmap ���� ����. 0���� ���� �� ������ ��� ũ��� �����Ѵ�.
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// 8-2. �ش� ������ �̿��Ͽ� ����-���ٽ� ���۰� �ۼ��� �ؽ���2D�� �����.
	hr = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilbuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// 8-3. ����ü �޸𸮸� �ʱ�ȭ�ϰ�, ����-���ٽ� �信 ���� ������ �ۼ��Ѵ�.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 8-4. ������ �ؽ��� ������ ������ �̿��Ͽ�, ����-���ٽ� �並 �����Ѵ�.
	hr = mDevice->CreateDepthStencilView(mDepthStencilbuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// 8-5. ������ ����-���ٽ� ���۸� �� ����(���� Ÿ�� ��)�� ��ģ��(Bind).
	//	������ ������������ Output Merger Stage �ܰ迡�� �۵��ϹǷ� �Լ��� OM�� �ٴ´�.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

#pragma endregion

#pragma region 9. ���̷�Ʈ X �ʱ�ȭ 6 - �� ��Ʈ ����

	// 9. �� ��Ʈ�� ȭ�� ũ��� �����Ѵ�.
	//	3D -> 2D ���� �������� ���̹Ƿ� �����Ͷ��������� ���ȴ�.

	// 9-1. �� ��Ʈ ����ü ����
	//	D3D11_VIEWPORT ����ü�� ���������, CD3D11_VIEWPORT�� Ŭ������ ��ü �����ϴ�. (ZeroMemory��?)
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;

	// 9-2. �� ��Ʈ ����. �����Ͷ������� ������ RS�� �ٴ´�.
	mDeviceContext->RSSetViewports(1, &viewport);

#pragma endregion

#pragma region 10. �����Ͷ����� ���� ���� �� ����

	// 10. �����Ͷ����� ���¸� �����Ѵ�.
	//	�ٰ����� � ���� �׸� ��, ���̾� ������ ���� ������, �ø� ��� �� �������� �������Ǵ� ����� ������ �� �ִ�.
	//	�⺻������ �����Ǿ� ������, ���� �����Ϸ��� ���� �����Ͷ����� ���¸� ������ �Ѵ�. ���⼱ �⺻���� �����ϰ� ������� �Ѵ�.
	
	// 10-1. �����Ͷ����� ���� ����ü �ۼ� (���������� C�� �ٿ� Ŭ������ ���� ����)
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

	// 10-2. �����Ͷ����� ���� ����
	hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(hr))
	{
		return false;
	}

	// 10-3. �����Ͷ����� ���� ����
	mDeviceContext->RSSetState(mRasterState);

#pragma endregion

#pragma region 11. ���(Matrix) ����

	// 11. ������ ���������ο��� ���Ǵ� ��Ʈ�������� �����Ѵ�.
	//	�Ϲ������� View Matrix, World Matrix, Projection Matrix, Orthographic Matrix�� �ִ�.
	//	�������� ���� ���̴��� ���޵� �� �ֵ��� �纻�� �����ϴ� �뵵�� �� Ŭ������ ����Ǹ�, XMMatrix �迭�� �Լ��� �����.
	//	�츮�� ���� ����� ��ġ�� ����ϴ� View Matrix�� ���� ī�޶󿡼� ó���ϰ�, �������� ������ �Ѵ�.

	// 11-1. ���� ��ǥ�� (World Matrix) �ʱ�ȭ
	//	������Ʈ�� ���� ��ȯ�̳� Transform ������ ���ȴ�.
	mWorldMatrix = XMMatrixIdentity();

	// 11-2. ���� ��� (Projection Matrix) ���� �� ����
	//	3D ����� 2D ����Ʈ �������� ��ȯ�ϴ� �� ���ȴ�.
	fieldOfView = 3.1415926f / 4.0f;
	screenAspect = (float)screenWidth / screenHeight;

	mProjectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// 11-3. ���� ���� ��� (Orthographic Matrix) ����
	//	UI���� 2D �׷��� ��ҿ� �۲� ���� �������� ���δ�.
	mOrthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

#pragma endregion
	
	return true;
}

