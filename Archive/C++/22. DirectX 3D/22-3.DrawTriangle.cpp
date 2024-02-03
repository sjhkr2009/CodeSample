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
	// 이 부분은 Input Assembler를 설정하는 부분이라 매 프레임 실행할 필요는 없지만,
	// 설정값이 서로 다른 도형을 여러 개 그린다면 이 부분을 여러 번 지정해야 한다.

	UINT stride = sizeof(Vertex);	// stride: 보폭. 각 데이터의 크기가 입력되며, 몇 바이트 단위로 읽을지 정하는 데 사용된다.
	UINT offset = 0;				// offset: 어디부터 읽을 지 (바이트 단위)

	mspDeviceContext->IASetVertexBuffers(0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset);
	mspDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//-----------------------------------------------------------------------------------------------
	mspDeviceContext->Draw(3, 0);	// Vertex 개수, 시작점을 입력하면 렌더링 파이프라인에 들어간다.
}

void DrawTriangle::InitTriangle()
{
	// 1. Vertex(여기서는 삼각형이므로 꼭지점 3개)을 생성한다. 왼손 좌표계 기준(시계 방향)으로 그려지니 순서에 주의.
	// Direct 3D의 좌표는 중심을 (0,0), 화면의 좌측 상단을 (1,1), 우측 하단을 (-1,-1)로 간주한다.
	Vertex vertices[] =
	{
		{0.0f, 0.5f, 0.0f, { 1.f, 0.f, 0.f, 1.f }},
		{0.5f, -0.5f, 0.0f, { 0.f, 1.f, 0.f, 1.f }},
		{-0.5f, -0.5f, 0.0f, { 0.f, 0.f, 1.f, 1.f }}
	};

	// 2. 버퍼에 대한 설명을 작성하고, 버퍼를 생성한다.
	CD3D11_BUFFER_DESC bd(
		sizeof(vertices),
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_USAGE_DYNAMIC,		// CPU/GPU 중 어디서 사용할 것인지 지정한다. DYNAMIC은 둘 다에서 접근가능하며, CPU가 쓰고 GPU가 읽게 된다.
		D3D11_CPU_ACCESS_WRITE
	);

	mspDevice->CreateBuffer(
		&bd,
		nullptr,	// 초기값 설정. 기본값은 '사용하지 않음' (모두 복사하는 형태)
		mspVertexBuffer.ReleaseAndGetAddressOf()
	);

	// 3. Vertex 데이터들을 버퍼로 복사한다.
	
	// CPU/GPU를 분리하여 하드웨어 가속을 풀로 사용하기 위해, Map/Unmap을 자주 사용하게 된다.
	// 버퍼에 데이터 입력은 CPU/GPU 동시 접근 시 에러가 발생할 수 있으므로 임계 구역으로 설정해야 한다.
	// 다이렉트X에서는 Map/Unmap으로 설정/해제한다. lock을 거는 것과 유사하다.
	D3D11_MAPPED_SUBRESOURCE ms;
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,							// 초기값 없음 (전부 읽음)
		D3D11_MAP_WRITE_DISCARD,	// 이미 있는 내용을 지우고(초기화하고) 쓰기 모드로 진입한다.
		0,
		&ms							// 매핑한 정보가 구조체에 들어오게 된다.
	);

	// 생성된 버퍼로 구조체 데이터들을 복사한다.
	memcpy(ms.pData, vertices, sizeof(vertices));

	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);
}

void DrawTriangle::InitPipeline()
{
	// 0. 셰이더 코드 뭉치가 들어갈 포인터 선언
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	// 1. 셰이더 컴파일 (Blob 생성)
	D3DCompileFromFile(
		L"VertexShader.hlsl",	// 폴더 내 HLSL파일 주소 입력
		nullptr,				// define 정보가 있는 경우, D3D_SHADER_MACRO를 넘겨준다. { define값 : 바뀔 값 }, { define값 : 바뀔 값 }, { define값 : 바뀔 값 }, ... 형태의 배열이 입력된다.
		nullptr,
		"main",					// 셰이더 진입점 이름 (여기서는 main 함수)
		"vs_4_0_level_9_3",		// 셰이더 언어 버전 (다렉 9~10은 'Shader Model 4'를 ,11에서는  5를 쓴다. 하위 버전을 쓰면 상위 버전 기능이 제한되나, 최소사양은 낮아진다. 여기선 4를 쓰기로 한다.
		0, 0,					// 컴파일 옵션. 고급 기능이므로 여기선 쓰지 않는다.
		spVS.ReleaseAndGetAddressOf(),	// 셰이더 코드가 들어갈 포인터의 주소
		nullptr					// 에러 메시지가 들어갈 포인터 주소. 여기선 에러가 안 난다고 가정하고 nullptr을 준다.
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
		spVS->GetBufferPointer(),	// 버퍼의 포인터
		spVS->GetBufferSize(),		// 버퍼의 크기
		nullptr,					// 링커. 여러 쉐이터 파일이 링크되어야 할 경우 쓰이며, 여기선 단독 파일로 쓰니 null로 설정.
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

	// 4. 입력 레이아웃 생성 (버텍스 구조 : HLSL Sementic)
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		// Sementic 이름, Sementic 인덱스, 포맷, 입력 슬롯, 데이터 시작점(offset/바이트 단위), 데이터 유형(인스턴싱 사용 유무), 인스턴싱
		//	입력 슬롯은 여러 개의 버텍스 정보를 넘길 때 슬롯(0~15번) 구분을 위해 사용하며, 여기선 사용하지 않는다. 인스턴싱도 사용하지 않는다.
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
