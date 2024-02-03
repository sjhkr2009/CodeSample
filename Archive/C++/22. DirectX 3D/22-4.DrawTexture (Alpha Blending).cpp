#include "DrawTexture.h"

// 텍스쳐 파일을 읽어오기 위한 fstream과 저장공간을 선언하기 위한 벡터를 가져온다.
#include <fstream>
#include <vector>

void DrawTexture::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitTriangle();

	CreateTextureFromBMP();
}

void DrawTexture::Destroy()
{
	mspBlendState.Reset();
	mspSamplerState.Reset();

	mspTextureView.Reset();
	mspTexture.Reset();

	mspVertexBuffer.Reset();
	mspInputLayout.Reset();
	mspPixelShader.Reset();
	mspVertexShader.Reset();

	D3DFramework::Destroy();
}

void DrawTexture::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mspDeviceContext->IASetVertexBuffers(0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset);
	mspDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mspDeviceContext->PSSetShaderResources(0, 1, mspTextureView.GetAddressOf());

	// 샘플러 상태 적용
	mspDeviceContext->PSSetSamplers(0, 1, mspSamplerState.GetAddressOf());
	// 알파 블렌딩 적용
	mspDeviceContext->OMSetBlendState(mspBlendState.Get(), nullptr, 0xFFFFFFFF);

	mspDeviceContext->Draw(4, 0);
}

void DrawTexture::InitTriangle()
{
	// 1. Vertex 생성
	//	UV 좌표계는 좌측 상단을 원점으로, 우측 하단을 (1,1)로 간주한다.
	//	border 외곽선 테스트를 위해 UV 좌표의 0~1이 아닌-0.05~1.05 까지 그려본다.
	Vertex vertices[] =
	{
		{-0.5f, 0.5f, 0.0f,		-0.05f, -0.05f},
		{0.5f, 0.5f, 0.0f,		1.05f, -0.05f},
		{-0.5f, -0.5f, 0.0f,	-0.05f, 1.05f},
		{0.5f, -0.5f, 0.0f,		1.05f, 1.05f}
	};

	// 2. 버퍼 생성
	CD3D11_BUFFER_DESC bd(
		sizeof(vertices),
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,
		D3D11_CPU_ACCESS_WRITE
	);

	mspDevice->CreateBuffer(
		&bd,
		nullptr,
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);

	// 3. Vertex 데이터들을 버퍼로 복사
	D3D11_MAPPED_SUBRESOURCE ms;
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);

	memcpy(ms.pData, vertices, sizeof(vertices));

	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);


	// + 샘플러 제어를 위한 인터페이스 생성

	float border[4]{ 0.f, 0.f, 0.f, 1.f };

	CD3D11_SAMPLER_DESC samplerDesc(
		D3D11_FILTER_MIN_MAG_MIP_POINT,	// 보간 방법
		D3D11_TEXTURE_ADDRESS_BORDER,	// u,v,w가 0~1 범위를 초과할 경우 처리방법
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_BORDER,
		0.0f,
		1,								// Arisomathic
		D3D11_COMPARISON_ALWAYS,
		border,							// 외곽선 색상 (보간 방법이 BORDER일 때, 0~1 초과 범위는 border 색상으로 지정된다)
		0,
		1
	);

	mspDevice->CreateSamplerState(
		&samplerDesc,
		mspSamplerState.ReleaseAndGetAddressOf()
	);

	// + 알파 블렌딩 제어를 위한 인터페이스 생성

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	// 블렌딩은 렌더 타겟의 소스(Dest)와 픽셀 셰이더 소스(Src)의 색상 값을 혼합하는 작업.
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;				// D3D11_BLEND_ONE : 원본 그대로
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// INV_SRC_ALPHA : 각 색상값에 (1 - Src의 알파값)을 곱한 값을 쓴다
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				// 두 색상값을 더하여 혼합한다

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// 픽셀 셰이더의 알파값은 그대로 쓰고
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		// 렌더 타겟의 알파값은 0으로 만든다(= 안 쓴다)
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		// 두 알파값을 더하여 혼합한다

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mspDevice->CreateBlendState(
		&blendDesc, mspBlendState.ReleaseAndGetAddressOf()
	);
}

void DrawTexture::InitPipeline()
{
	// 0. 셰이더 코드 뭉치가 들어갈 포인터 선언
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// 1. 셰이더 컴파일 (Blob 생성)
	D3DCompileFromFile(
		L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_4_0_level_9_3",
		0, 0,
		spVS.ReleaseAndGetAddressOf(),
		nullptr
	);
	D3DCompileFromFile(
		L"PixelShader.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_4_0_level_9_3",
		0, 0,
		spPS.ReleaseAndGetAddressOf(),
		nullptr
	);

	// 2. 셰이더 생성 (블롭 정보 이용)
	mspDevice->CreateVertexShader(
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		nullptr,
		mspVertexShader.ReleaseAndGetAddressOf()
	);
	mspDevice->CreatePixelShader(
		spPS->GetBufferPointer(),
		spPS->GetBufferSize(),
		nullptr,
		mspPixelShader.ReleaseAndGetAddressOf()
	);

	// 3. 장치에 셰이더 연결
	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// 4. 입력 레이아웃 생성
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		// 색상이 텍스쳐로 바뀌었으므로, 해당 부분만 변경한다. U,V 두 가지 정보만 있으니 R32 G32 형태로 읽는다.
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}	// "TEXCOORD0"은 입력 레이아웃에서 "TEXCOORD", 0 형태로 입력.
	};
	mspDevice->CreateInputLayout(
		ied,
		2,
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		mspInputLayout.ReleaseAndGetAddressOf()
	);
	mspDeviceContext->IASetInputLayout(mspInputLayout.Get());

}

HRESULT DrawTexture::CreateTextureFromBMP()
{
	// 1. BMP 파일 로딩
	// 설명은 19-9번 파일의 ImageExample::LoadBMP() 참고
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeaher;

	file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
	file.read(reinterpret_cast<char*>(&infoHeaher), sizeof(BITMAPINFOHEADER));

	if ((fileHeader.bfType != 0x4D42) || (infoHeaher.biBitCount != 32))
	{
		return E_FAIL;
	}

	std::vector<char> pPixels(infoHeaher.biSizeImage);
	file.seekg(fileHeader.bfOffBits);

	int pitch = infoHeaher.biWidth * (infoHeaher.biBitCount / 8);

	// 배경색 제거 (Alpha Blending)
	//---------------------------------------------------------------------------------------------------

	/*for (int y = infoHeaher.biHeight - 1; y >= 0; --y)
	{
		file.read(&pPixels[y * pitch], pitch);
	}*/

	int index{};
	char r{}, g{}, b{}, a{};
	for (int y = infoHeaher.biHeight - 1; y >= 0; --y)
	{
		index = y * pitch;
		for (int x = 0; x < infoHeaher.biWidth; x++)
		{
			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if ((unsigned char)r == 30 &&
				(unsigned char)g == 199 &&
				(unsigned char)b == 250)
			{
				pPixels[index] = pPixels[index + 1] = pPixels[index + 2] = pPixels[index + 3] = 0;
			}
			else
			{
				pPixels[index] = b;
				pPixels[index + 1] = g;
				pPixels[index + 2] = r;
				pPixels[index + 3] = a;
			}
			index += 4;
		}
	}
	// 이렇게 배경색과 일치하면 알파값을 빼 줘도, 배경색은 검은색이 될 뿐 알파값이 빠지지 않는다.
	// 알파값을 조작하려면 알파 블렌딩을 활성화해야 한다.

	//---------------------------------------------------------------------------------------------------

	file.close();


	// 2. 텍스쳐 생성
	CD3D11_TEXTURE2D_DESC texDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		infoHeaher.biWidth, infoHeaher.biHeight,
		1, 1
	);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem			= (void*)(&pPixels[0]);
	initData.SysMemPitch		= pitch;
	initData.SysMemSlicePitch	= 0;	// Z축에 대한 pitch. 여기서는 쓰지 않음.

	mspDevice->CreateTexture2D(
		&texDesc,
		&initData,
		mspTexture.ReleaseAndGetAddressOf()
	);

	// 3. 셰이더 뷰 생성
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		texDesc.Format,
		0, 1
	);

	mspDevice->CreateShaderResourceView(
		mspTexture.Get(),
		&srvDesc,
		mspTextureView.ReleaseAndGetAddressOf()
	);
	
	return S_OK;
}
