#include "DrawTriangle.h"

void DrawTriangle::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitTriangle();
}

void DrawTriangle::Destroy()
{
	mspVertexBuffer.Reset();
	mspInputLayout.Reset();
	mspPixelShader.Reset();
	mspVertexShader.Reset();

	D3DFramework::Destroy();
}

void DrawTriangle::Render()
{
	//-----------------------------------------------------------------------------------------------
	// �� �κ��� Input Assembler�� �����ϴ� �κ��̶� �� ������ ������ �ʿ�� ������,
	// �������� ���� �ٸ� ������ ���� �� �׸��ٸ� �� �κ��� ���� �� �����ؾ� �Ѵ�.

	UINT stride = sizeof(Vertex);	// stride: ����. �� �������� ũ�Ⱑ �ԷµǸ�, �� ����Ʈ ������ ������ ���ϴ� �� ���ȴ�.
	UINT offset = 0;				// offset: ������ ���� �� (����Ʈ ����)

	mspDeviceContext->IASetVertexBuffers(0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset);
	mspDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//-----------------------------------------------------------------------------------------------
	mspDeviceContext->Draw(3, 0);	// Vertex ����, �������� �Է��ϸ� ������ ���������ο� ����.
}

void DrawTriangle::InitTriangle()
{
	// 1. Vertex(���⼭�� �ﰢ���̹Ƿ� ������ 3��)�� �����Ѵ�. �޼� ��ǥ�� ����(�ð� ����)���� �׷����� ������ ����.
	// Direct 3D�� ��ǥ�� �߽��� (0,0), ȭ���� ���� ����� (1,1), ���� �ϴ��� (-1,-1)�� �����Ѵ�.
	Vertex vertices[] =
	{
		{0.0f, 0.5f, 0.0f, { 1.f, 0.f, 0.f, 1.f }},
		{0.5f, -0.5f, 0.0f, { 0.f, 1.f, 0.f, 1.f }},
		{-0.5f, -0.5f, 0.0f, { 0.f, 0.f, 1.f, 1.f }}
	};

	// 2. ���ۿ� ���� ������ �ۼ��ϰ�, ���۸� �����Ѵ�.
	CD3D11_BUFFER_DESC bd(
		sizeof(vertices),
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,		// CPU/GPU �� ��� ����� ������ �����Ѵ�. DYNAMIC�� �� �ٿ��� ���ٰ����ϸ�, CPU�� ���� GPU�� �а� �ȴ�.
		D3D11_CPU_ACCESS_WRITE
	);

	mspDevice->CreateBuffer(
		&bd,
		nullptr,	// �ʱⰪ ����. �⺻���� '������� ����' (��� �����ϴ� ����)
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);

	// 3. Vertex �����͵��� ���۷� �����Ѵ�.
	
	// CPU/GPU�� �и��Ͽ� �ϵ���� ������ Ǯ�� ����ϱ� ����, Map/Unmap�� ���� ����ϰ� �ȴ�.
	// ���ۿ� ������ �Է��� CPU/GPU ���� ���� �� ������ �߻��� �� �����Ƿ� �Ӱ� �������� �����ؾ� �Ѵ�.
	// ���̷�ƮX������ Map/Unmap���� ����/�����Ѵ�. lock�� �Ŵ� �Ͱ� �����ϴ�.
	D3D11_MAPPED_SUBRESOURCE ms;
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,							// �ʱⰪ ���� (���� ����)
		D3D11_MAP_WRITE_DISCARD,	// �̹� �ִ� ������ �����(�ʱ�ȭ�ϰ�) ���� ���� �����Ѵ�.
		0,
		&ms							// ������ ������ ����ü�� ������ �ȴ�.
	);

	// ������ ���۷� ����ü �����͵��� �����Ѵ�.
	memcpy(ms.pData, vertices, sizeof(vertices));

	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);
}

void DrawTriangle::InitPipeline()
{
	// 0. ���̴� �ڵ� ��ġ�� �� ������ ����
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// 1. ���̴� ������ (Blob ����)
	D3DCompileFromFile(
		L"VertexShader.hlsl",	// ���� �� HLSL���� �ּ� �Է�
		nullptr,				// define ������ �ִ� ���, D3D_SHADER_MACRO�� �Ѱ��ش�. { define�� : �ٲ� �� }, { define�� : �ٲ� �� }, { define�� : �ٲ� �� }, ... ������ �迭�� �Էµȴ�.
		nullptr,
		"main",					// ���̴� ������ �̸� (���⼭�� main �Լ�)
		"vs_4_0_level_9_3",		// ���̴� ��� ���� (�ٷ� 9~10�� 'Shader Model 4'�� ,11������  5�� ����. ���� ������ ���� ���� ���� ����� ���ѵǳ�, �ּһ���� ��������. ���⼱ 4�� ����� �Ѵ�.
		0, 0,					// ������ �ɼ�. ��� ����̹Ƿ� ���⼱ ���� �ʴ´�.
		spVS.ReleaseAndGetAddressOf(),	// ���̴� �ڵ尡 �� �������� �ּ�
		nullptr					// ���� �޽����� �� ������ �ּ�. ���⼱ ������ �� ���ٰ� �����ϰ� nullptr�� �ش�.
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
		spVS->GetBufferPointer(),	// ������ ������
		spVS->GetBufferSize(),		// ������ ũ��
		nullptr,					// ��Ŀ. ���� ������ ������ ��ũ�Ǿ�� �� ��� ���̸�, ���⼱ �ܵ� ���Ϸ� ���� null�� ����.
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

	// 4. �Է� ���̾ƿ� ���� (���ؽ� ���� : HLSL Sementic)
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		// Sementic �̸�, Sementic �ε���, ����, �Է� ����, ������ ������(offset/����Ʈ ����), ������ ����(�ν��Ͻ� ��� ����), �ν��Ͻ�
		//	�Է� ������ ���� ���� ���ؽ� ������ �ѱ� �� ����(0~15��) ������ ���� ����ϸ�, ���⼱ ������� �ʴ´�. �ν��Ͻ̵� ������� �ʴ´�.
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
