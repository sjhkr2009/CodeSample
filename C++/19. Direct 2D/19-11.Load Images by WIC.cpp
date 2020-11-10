#include "ImageExample.h"
#include <fstream>
#include <vector>

// WIC�� ������ �ڵ� ���̺귯���� �־�� �����Ѵ�.
#pragma comment (lib, "WindowsCodecs.lib")

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	// WIC�� ����Ͽ�, DIrectX�� ������ ��κ��� COM������Ʈ�� �����츦 ���� �۵��Ѵ�.
	// ���� ��� COM ��ü�� ���� �ʱ�ȭ �� ������ �ʿ��ϴ�. (CoInitialize / CoUnInitialize)
	CoInitialize(nullptr);

	// DirectX�� �ʱ�ȭ�� �Ϻ� ���۵��� �ڵ����� ó��������, WIC�� �׷� ���� ����� ����.
	// ���� �����쿡���� COM ǥ�� ������ ���� �ʱ�ȭ -> �ν��Ͻ�ȭ ������ ����� �Ѵ�.
	CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,		// ���������� ����-Ŭ���̾�Ʈ ������� �۵��Ѵ�.
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
		D2D1_RECT_F{ 0, 0, 1024, 768 },		// �̹����� ����� ������ ũ��
		1.0f,								// ������
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);	// ���� ���

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
	// WIC�� ���� ��� �̹��� ���� ������ ������ �� �־ �ټ� �����ϴ�.

	//  * ���⼭ ��Ʈ���� .bmp ������ �ƴ϶� ���� �̹����� ���Ǵ� ����.

// 1. ���ڴ� (Decoder) ���� : � ������ ������ �а� �ؼ��ϴ°��� ���� ������ ��� ����

	// ��Ʈ�� ���ڴ��� �����ϰ�, ���丮�� ���� ������ش�.
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> bitmapDecoder;

	HRESULT hr = mspWICFactory->CreateDecoderFromFilename(
		filename,
		nullptr,		// GUID(Globally Unique IDentifier, �����迡 �ϳ����� �ĺ���(ID)) Vendor(������). ID�� �ʿ��� ���� �����̸� �ʿ�������, ��Ʈ���� �����̹Ƿ� ���ʿ��ϴ�.
		GENERIC_READ,	// �б� ���
		WICDecodeMetadataCacheOnLoad, // ��Ÿ������ �ε� �ɼ�. ��Ÿ�����ʹ� ������ infoHeader�� ���� ������ ��ü�� ���� ���� ������ ��� �ִ�.
		bitmapDecoder.GetAddressOf()
	);
	ThrowIfFailed(hr);

// 2. ���ڴ����� ������(Frame)�� �����´�. (Ư��, ���� ������ ������ �����̳� �ִϸ��̼� gif ������ ���)
	
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;

	// ���⼭�� ������ 1�常 �������� �ǹǷ� �ε��� 0�� �����Ӹ� �����´�.
	ThrowIfFailed( bitmapDecoder->GetFrame(0, frame.GetAddressOf()) );

// 3. ������ �����͸� ��ȯ(Converter)�Ѵ�.

	// ������ ���� �� �ʱ�ȭ
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	ThrowIfFailed( mspWICFactory->CreateFormatConverter(converter.GetAddressOf()) );

	hr = converter->Initialize(
		frame.Get(),	// �̹��� �ҽ�
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,	// ����� ��� ���. ���⼭�� ���� �ʴ°ɷ� �Ѵ�.
		nullptr,					// �ȷ�Ʈ. ���⼭�� ������� �ʴ´�.
		0,			// ���� �Ѱ���(threshold). �� ��ġ ������ ���İ��� �����ϰ� ó���Ѵ�.
		WICBitmapPaletteTypeCustom	// Ŀ���� �ȷ�Ʈ ���
	);
	ThrowIfFailed(hr);

// 4. ��ȯ�� �����͸� ID2D1Bitmap ���� �����Ѵ�.

	std::vector<char> image;

	// WIC�� ����� ��Ʈ���� �������� �Լ��� �̹� ���ǵǾ� �ִ�.
	hr = mspRenderTarget->CreateBitmapFromWicBitmap(
		converter.Get(),
		ppBitmap
	);
	ThrowIfFailed(hr);

	return S_OK;
}

