#pragma once
#include "D2DFramework.h"
#include <wincodec.h>

// WIC(Windows Imaging Component)�� �̿��� ��Ʈ�� �̹��� ����
// wincodec.h ��� �ʿ�

class ImageExample : public D2DFramework
{
private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap{};
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
