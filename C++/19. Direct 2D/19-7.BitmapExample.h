#pragma once
#include "D2DFramework.h"

// 가상의 그래픽카드 구현 (1024 x 768)

const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 };

class BitmapExample : public D2DFramework
{
private:
    // 테스트 출력용
    Microsoft::WRL::ComPtr<ID2D1Bitmap>     mspFrameBitmap{};
    // 가상 그래픽 카드 (포인터로 선언해야 동적 할당이 가능하며, 힙 메모리에 생성된다.)
    std::unique_ptr<UINT8[]>                mspBackBuffer{};

public:
    virtual HRESULT Initialize(HINSTANCE hInstance,
        LPCWSTR title = L"Direct2D Example",
        UINT width = 1024,
        UINT height = 768
    ) override;

    virtual void Render() override;

    void ClearBuffer(D2D1::ColorF color);
    void PresentBuffer();
    void DrawPixelToBuffer(int x, int y, D2D1::ColorF color);
    void FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color);

};

