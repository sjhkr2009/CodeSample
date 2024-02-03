#pragma once

#include <wrl/client.h>	// COM ��ü�� ���� ������(ComPtr)�� ����ϱ� ���� ���
#include <d2d1.h>		// Direct 2D ����� ���� ���
#include <wincodec.h>	// WIC ����� ���� ���
#include <map>			// �̹����� ������ �����̳ʷ�, Key���� �̹��� ������ ��θ� ����Ѵ�.
#include <string>		// �̹��� �����̳��� Key���� ���� ��η� �Է��ؾ� �ϹǷ� wstring ����� ���� ���

class BitmapManager final
{
public:
	static BitmapManager& Instance()
	{
		static BitmapManager instance;
		return instance;
	}

private:
	BitmapManager() {}
	BitmapManager(const BitmapManager&) {}
	void operator =(const BitmapManager&) {}

public:
	~BitmapManager() {}

private:
	ID2D1HwndRenderTarget*										mpRenderTarget{};
	Microsoft::WRL::ComPtr<IWICImagingFactory>					mspWICFactory{};
	std::map<std::wstring, Microsoft::WRL::ComPtr<ID2D1Bitmap>> mBitmapResources{};

public:
	HRESULT Initialize(ID2D1HwndRenderTarget* pRT);
	void Release();
	// ����: ������/�Ҹ��� ��� �ʱ�ȭ(Init)/����(Release)�� ���� ���� �ڿ��� �����ϴ� ������ 'RAII' �����̶�� �Ѵ�.
	//	�� RAII: Resource Acquisition is Initialization

	ID2D1Bitmap* LoadBitmap(std::wstring filename);

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);
	
};

