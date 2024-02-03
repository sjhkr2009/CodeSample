#pragma once

#include <wrl/client.h>	// COM 객체를 위한 포인터(ComPtr)를 사용하기 위한 헤더
#include <d2d1.h>		// Direct 2D 사용을 위한 헤더
#include <wincodec.h>	// WIC 사용을 위한 헤더
#include <map>			// 이미지를 저장할 컨테이너로, Key값은 이미지 파일의 경로를 사용한다.
#include <string>		// 이미지 컨테이너의 Key값을 파일 경로로 입력해야 하므로 wstring 사용을 위한 헤더

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
	// 참고: 생성자/소멸자 대신 초기화(Init)/해제(Release)를 따로 만들어서 자원을 생성하는 개념을 'RAII' 패턴이라고 한다.
	//	ㄴ RAII: Resource Acquisition is Initialization

	ID2D1Bitmap* LoadBitmap(std::wstring filename);

private:
	HRESULT LoadWICBitmap(std::wstring filename, ID2D1Bitmap** ppBitmap);
	
};

