#pragma once
#include "D2DFramework.h"
#include <wincodec.h>

// WIC(Windows Imaging Component)를 이용한 비트맵 이미지 열기
//  * 여기서 비트맵은 .bmp 파일이 아니라 벡터 이미지와 대비되는 개념.
// wincodec.h 헤더 필요

class ImageExample : public D2DFramework
{
private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap{};

    // 이미지를 여러 개 생성할때는 비트맵 포인터도 해당 개수만큼 필요하다.
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmapBackground{};
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmapCharacter{};
    Microsoft::WRL::ComPtr<IWICImagingFactory> mspWICFactory{};

public:
    virtual HRESULT	Initialize(HINSTANCE hInstance,
        LPCWSTR title = L"Direct2D Example",
        UINT width = 1024,
        UINT height = 768
    ) override;
    virtual void Release() override;
    virtual void Render() override;

    HRESULT LoadWICImage(LPCWSTR filename, ID2D1Bitmap** ppBitmap);

};

