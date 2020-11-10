#include "BitmapManager.h"
#include "D2DFramework.h"

#pragma comment (lib, "WindowsCodecs.lib")

using namespace Microsoft::WRL;

HRESULT BitmapManager::Initialize(ID2D1HwndRenderTarget* pRT)
{
	if (pRT == nullptr)
	{
		return E_FAIL;
	}

	mpRenderTarget = pRT;
	
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(mspWICFactory.GetAddressOf())
	);
	ThrowIfFailed(hr);
    
    return S_OK;
}

void BitmapManager::Release()
{
	for (auto& e : mBitmapResources)
	{
		e.second.Reset();
	}
	mBitmapResources.clear();
	
	mspWICFactory.Reset();
	
}

ID2D1Bitmap* BitmapManager::LoadBitmap(std::wstring filename)
{
	if (mspWICFactory == nullptr || mpRenderTarget == nullptr)
	{
		return nullptr;
	}

	// 오버헤드 방지를 위해, map에 해당 원소가 있는지 확인(find)한 후 추가 동작을 실행하는 대신, 아래와 같은 방식을 이용한다.
	// map을 두 번 탐색하는 대신 한 번만 탐색할 수 있다.

	// map의 insert는 pair<iterator, bool> 형태를 반환한다.
	// iterator는 삽입 위치(이미 Key가 있다면 해당 위치), bool은 삽입 성공 여부.
	auto result = mBitmapResources.insert(
		std::pair<std::wstring, ComPtr<ID2D1Bitmap>>(filename, nullptr)
	);

	if (result.second == true)
	{
		ComPtr<ID2D1Bitmap> spBitmap{};
		LoadWICBitmap(filename, spBitmap.GetAddressOf());

		// result.first는 추가된 위치를 가리키는 포인터이므로, 해당 위치의 Value인 second에 비트맵을 추가한다.
		result.first->second = spBitmap;
	}
	
	return result.first->second.Get();
}

HRESULT BitmapManager::LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap)
{
	ComPtr<IWICBitmapDecoder> bitmapDecoder;
	HRESULT hr;

	auto pFactory{ mspWICFactory.Get() };
	if (pFactory == nullptr)
	{
		return E_FAIL;
	}

	hr = pFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

	ComPtr<IWICBitmapFrameDecode> frame;
	ThrowIfFailed(bitmapDecoder->GetFrame(0, frame.GetAddressOf()));

	ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed(pFactory->CreateFormatConverter(converter.GetAddressOf()));

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0,
		WICBitmapPaletteTypeCustom
	);
	ThrowIfFailed(hr);

	hr = mpRenderTarget
		->CreateBitmapFromWicBitmap(converter.Get(), ppBitmap);
	ThrowIfFailed(hr);

	return S_OK;
}
