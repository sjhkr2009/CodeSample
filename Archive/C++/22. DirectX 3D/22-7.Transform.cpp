#include "DrawTexture.h"

// �ؽ��� ������ �о���� ���� fstream�� ��������� �����ϱ� ���� ���͸� �����´�.
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

	mspDeviceContext->PSSetSamplers(0, 1, mspSamplerState.GetAddressOf());
	mspDeviceContext->OMSetBlendState(mspBlendState.Get(), nullptr, 0xFFFFFFFF);

	MatrixBuffer mb;
	mb.world = DirectX::XMMatrixTranspose(mWorld);
	mspDeviceContext->UpdateSubresource(mspConstBuffer.Get(),
		0,
		nullptr,	// �ٲ� ������ ũ��. nullptr�̸� ��ü �� �ٲ۴�.
		&mb,
		0, 0		// �ٲ� ������ ���� ũ��� ����. ��ü �� �ٲܶ��� ������ �ʿ� ����.
	);

	mspDeviceContext->Draw(4, 0);
}

void DrawTexture::Update(float delta)
{
	if (mInput.IsKeyDown('Q'))
	{
		mRotationZ -= DirectX::XM_PI * delta;	// �ʴ� ����(180��)��ŭ ȸ����Ų��.
	}
	else if (mInput.IsKeyDown('E'))
	{
		mRotationZ += DirectX::XM_PI * delta;
	}

	if (mInput.IsKeyDown('A'))
	{
		mX -= 1.0f * delta;
	}
	else if (mInput.IsKeyDown('D'))
	{
		mX += 1.0f * delta;
	}

	if (mInput.IsKeyDown('S'))
	{
		mY -= 1.0f * delta;
	}
	else if (mInput.IsKeyDown('W'))
	{
		mY += 1.0f * delta;
	}
	
	if (mInput.IsKeyDown('1'))
	{
		mTimer.SetScale(1.f);
	}
	else if (mInput.IsKeyDown('2'))
	{
		mTimer.SetScale(2.f);
	}
	else if (mInput.IsKeyDown('3'))
	{
		mTimer.SetScale(3.f);
	}
	
	
	mWorld = DirectX::XMMatrixIdentity();	// ���� ���(�ʱ�ȭ��)
	
	// ����� ���ϱ� ������ �����ؾ� �Ѵ�.
	mWorld *= DirectX::XMMatrixRotationZ(mRotationZ);
	mWorld *= DirectX::XMMatrixTranslation(mX, mY, 0.0f);
}

void DrawTexture::InitTriangle()
{
	// 1. Vertex ����
	//	UV ��ǥ��� ���� ����� ��������, ���� �ϴ��� (1,1)�� �����Ѵ�.
	Vertex vertices[] =
	{
		{-0.5f, 0.5f, 0.0f,		-0.0f, -0.0f},
		{0.5f, 0.5f, 0.0f,		1.0f, -0.0f},
		{-0.5f, -0.5f, 0.0f,	-0.0f, 1.0f},
		{0.5f, -0.5f, 0.0f,		1.0f, 1.0f}
	};

	// 2. ���� ����
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

	// 3. Vertex �����͵��� ���۷� ����
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


	// + ���÷� ��� ���� �������̽� ����

	float border[4]{ 0.f, 0.f, 0.f, 1.f };

	CD3D11_SAMPLER_DESC samplerDesc(
		D3D11_FILTER_MIN_MAG_MIP_POINT,	// ���� ���
		D3D11_TEXTURE_ADDRESS_BORDER,	// u,v,w�� 0~1 ������ �ʰ��� ��� ó�����
		D3D11_TEXTURE_ADDRESS_BORDER,
		D3D11_TEXTURE_ADDRESS_BORDER,
		0.0f,
		1,								// Arisomathic
		D3D11_COMPARISON_ALWAYS,
		border,							// �ܰ��� ���� (���� ����� BORDER�� ��, 0~1 �ʰ� ������ border �������� �����ȴ�)
		0,
		1
	);

	mspDevice->CreateSamplerState(
		&samplerDesc,
		mspSamplerState.ReleaseAndGetAddressOf()
	);

	// + ���� ���� ��� ���� �������̽� ����

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	// ������ ���� Ÿ���� �ҽ�(Dest)�� �ȼ� ���̴� �ҽ�(Src)�� ���� ���� ȥ���ϴ� �۾�.
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;				// D3D11_BLEND_ONE : ���� �״��
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	// INV_SRC_ALPHA : �� ���󰪿� (1 - Src�� ���İ�)�� ���� ���� ����
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				// �� ������ ���Ͽ� ȥ���Ѵ�

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			// �ȼ� ���̴��� ���İ��� �״�� ����
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		// ���� Ÿ���� ���İ��� 0���� �����(= �� ����)
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;		// �� ���İ��� ���Ͽ� ȥ���Ѵ�

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mspDevice->CreateBlendState(&blendDesc, mspBlendState.ReleaseAndGetAddressOf());

	// * ���ؽ� ���̴��� �ѱ� ��� ������ ���� ��� ���� ����

	// ����ü ������ ��� ���۸� ���� ���·� �������Ѵ�.
	bd = CD3D11_BUFFER_DESC(
		sizeof(MatrixBuffer),
		D3D11_BIND_CONSTANT_BUFFER
		// �뵵(Usage)�� �⺻���� Default�� �д�. ���� ���� Dynamic���� �ᵵ ������ ����Ʈ�� �����ϹǷ� ����� ����.
	);

	// Dynamic Usage:	CPU�� ����, GPU�� �б�		/	Map,Unmap���� ���			/	�� ������ �ٲ�� �����Ϳ� ����(���ؽ� ����, ��� ����)
	// Default Usage:	GPU�� �б�,���� ��� ����	/	UpdateSubResource�� ���	/	�ѹ� ����ϰ� �� �ٲ��� �ʴ� ��ҿ� ����(�ؽ��� ��)
	mspDevice->CreateBuffer(&bd, nullptr, mspConstBuffer.ReleaseAndGetAddressOf());
	mspDeviceContext->VSSetConstantBuffers(0, 1, mspConstBuffer.GetAddressOf());

	mX = mY = 0.f;
	mRotationZ = 0.f;
}

void DrawTexture::InitPipeline()
{
	// 0. ���̴� �ڵ� ��ġ�� �� ������ ����
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// 1. ���̴� ������ (Blob ����)
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

	// 2. ���̴� ���� (��� ���� �̿�)
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

	// 3. ��ġ�� ���̴� ����
	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// 4. �Է� ���̾ƿ� ����
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		// ������ �ؽ��ķ� �ٲ�����Ƿ�, �ش� �κи� �����Ѵ�. U,V �� ���� ������ ������ R32 G32 ���·� �д´�.
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	// 1. BMP ���� �ε�
	// ������ 19-9�� ������ ImageExample::LoadBMP() ����
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

	// ���� ���� (Alpha Blending)
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
	// �̷��� ������ ��ġ�ϸ� ���İ��� �� �൵, ������ �������� �� �� ���İ��� ������ �ʴ´�.
	// ���İ��� �����Ϸ��� ���� ������ Ȱ��ȭ�ؾ� �Ѵ�.

	//---------------------------------------------------------------------------------------------------

	file.close();


	// 2. �ؽ��� ����
	CD3D11_TEXTURE2D_DESC texDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		infoHeaher.biWidth, infoHeaher.biHeight,
		1, 1
	);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem			= (void*)(&pPixels[0]);
	initData.SysMemPitch		= pitch;
	initData.SysMemSlicePitch	= 0;	// Z�࿡ ���� pitch. ���⼭�� ���� ����.

	mspDevice->CreateTexture2D(
		&texDesc,
		&initData,
		mspTexture.ReleaseAndGetAddressOf()
	);

	// 3. ���̴� �� ����
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
