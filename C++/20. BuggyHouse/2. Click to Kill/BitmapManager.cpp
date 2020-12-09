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

	// ������� ������ ����, map�� �ش� ���Ұ� �ִ��� Ȯ��(find)�� �� �߰� ������ �����ϴ� ���, �Ʒ��� ���� ����� �̿��Ѵ�.
	// map�� �� �� Ž���ϴ� ��� �� ���� Ž���� �� �ִ�.

	// map�� insert�� pair<iterator, bool> ���¸� ��ȯ�Ѵ�.
	// iterator�� ���� ��ġ(�̹� Key�� �ִٸ� �ش� ��ġ), bool�� ���� ���� ����.
	auto result = mBitmapResources.insert(
		std::pair<std::wstring, ComPtr<ID2D1Bitmap>>(filename, nullptr)
	);

	if (result.second == true)
	{
		ComPtr<ID2D1Bitmap> spBitmap{};
		LoadWICBitmap(filename, spBitmap.GetAddressOf());

		// result.first�� �߰��� ��ġ�� ����Ű�� �������̹Ƿ�, �ش� ��ġ�� Value�� second�� ��Ʈ���� �߰��Ѵ�.
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
