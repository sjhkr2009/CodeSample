#include "ImageExample.h"
#include <fstream>
#include <vector>

// WIC는 윈도우 코덱 라이브러리가 있어야 동작한다.
#pragma comment (lib, "WindowsCodecs.lib")

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	// WIC를 비롯하여, DIrectX를 제외한 대부분의 COM오브젝트는 윈도우를 통해 작동한다.
	// 따라서 모든 COM 객체에 대한 초기화 및 해제가 필요하다. (CoInitialize / CoUnInitialize)
	CoInitialize(nullptr);

	// DirectX는 초기화와 일부 동작들이 자동으로 처리되지만, WIC는 그런 편의 기능이 없다.
	// 따라서 윈도우에서의 COM 표준 사용법을 따라 초기화 -> 인스턴스화 과정을 따라야 한다.
	CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,		// 내부적으로 서버-클라이언트 방식으로 작동한다.
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	
	D2DFramework::Initialize(hInstance, title, width, height);

	HRESULT hr;
	hr = LoadWICImage(L"Data/background.jpg", &mspBitmapBackground);
	hr = LoadWICImage(L"Data/player.png", &mspBitmapCharacter);

	return hr;
}

void ImageExample::Release()
{
	D2DFramework::Release();

	mspWICFactory.Reset();
	mspBitmap.Reset();
	mspBitmapBackground.Reset();
	mspBitmapCharacter.Reset();

	CoUninitialize();
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));
	
	
	mspRenderTarget->DrawBitmap(mspBitmapBackground.Get(),
		D2D1_RECT_F{ 0, 0, 1024, 768 },		// 이미지를 출력할 공간의 크기
		1.0f,								// 불투명도
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);	// 보간 방법

	mspRenderTarget->DrawBitmap(mspBitmapCharacter.Get(),
		D2D1_RECT_F{ 300, 300, 420, 450 },
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	//mspRenderTarget->DrawBitmap(mspBitmapCharacter.Get());

	HRESULT hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

HRESULT ImageExample::LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
	// WIC는 거의 모든 이미지 파일 포맷을 가져올 수 있어서 다소 복잡하다.

	//  * 여기서 비트맵은 .bmp 파일이 아니라 벡터 이미지와 대비되는 개념.

// 1. 디코더 (Decoder) 생성 : 어떤 식으로 파일을 읽고 해석하는가에 대한 정보를 담고 있음

	// 비트맵 디코더를 선언하고, 팩토리를 통해 만들어준다.
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;

	HRESULT hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,		// GUID(Globally Unique IDentifier, 전세계에 하나뿐인 식별자(ID)) Vendor(제조사). ID가 필요한 파일 형식이면 필요하지만, 비트맵은 무료이므로 불필요하다.
		GENERIC_READ,	// 읽기 모드
		WICDecodeMetadataCacheOnLoad, // 메타데이터 로딩 옵션. 메타데이터는 이전의 infoHeader와 같이 데이터 자체에 대한 저장 정보를 담고 있다.
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

// 2. 디코더에서 프레임(Frame)을 가져온다. (특히, 여러 장으로 구성된 사진이나 애니메이션 gif 파일인 경우)
	
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;

	// 여기서는 프레임 1장만 가져오면 되므로 인덱스 0번 프레임만 가져온다.
	ThrowIfFailed( bitmapDecoder->GetFrame(0, frame.GetAddressOf()) );

// 3. 프레임 데이터를 변환(Converter)한다.

	// 컨버터 생성 및 초기화
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed( mspWICFactory->CreateFormatConverter(converter.GetAddressOf()) );

	hr = converter->Initialize(
		frame.Get(),	// 이미지 소스
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,	// 디더링 사용 방식. 여기서는 쓰지 않는걸로 한다.
		nullptr,					// 팔레트. 여기서는 사용하지 않는다.
		0,			// 알파 한계점(threshold). 이 수치 이하의 알파값은 투명하게 처리한다.
		WICBitmapPaletteTypeCustom	// 커스텀 팔레트 사용
	);
	ThrowIfFailed(hr);

// 4. 변환한 데이터를 ID2D1Bitmap 으로 복사한다.

	std::vector<char> image;

	// WIC로 저장된 비트맵을 가져오는 함수가 이미 정의되어 있다.
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

