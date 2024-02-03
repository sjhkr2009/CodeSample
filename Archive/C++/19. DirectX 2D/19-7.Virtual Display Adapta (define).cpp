#include "BitmapExample.h"

HRESULT BitmapExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
    HRESULT hr = D2DFramework::Initialize(hInstance, title, width, height);
    ThrowIfFailed(hr);

    mspBackBuffer = std::make_unique<UINT8[]>(BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT);

    hr = mspRenderTarget->CreateBitmap(
        D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
        mspFrameBitmap.GetAddressOf()
    );
    ThrowIfFailed(hr);

    return hr;
}

void BitmapExample::Render()
{
    mspRenderTarget->BeginDraw();
    mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));
    
    ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightCyan));

    FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Red);
    FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(D2D1::ColorF::LightGreen, 0.5f));

    PresentBuffer();

    mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

    mspRenderTarget->EndDraw();
}

void BitmapExample::ClearBuffer(D2D1::ColorF color)
{
    UINT8* pCurrent = &mspBackBuffer[0];
    
    for (int i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; ++i)
    {
        *pCurrent = static_cast<UINT8>(color.r * 255);
        *(pCurrent + 1) = static_cast<UINT8>(color.g * 255);
        *(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
        *(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

        pCurrent += BITMAP_BYTECOUNT;
    }
}

void BitmapExample::PresentBuffer()
{
    // CopyFromMemory: �޸𸮸� ������ �´�. �Ű������� (������ ũ��, ������ �޸� ������, ������ �ϳ��� �޸� ũ��)
    //                  * ������ ũ�⿡ nullptr�� �Է��ϸ� ��ü�� ������ �´�.
    mspFrameBitmap->CopyFromMemory(nullptr, mspBackBuffer.get(), BITMAP_WIDTH * BITMAP_BYTECOUNT);
}

void BitmapExample::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
    // 2���� -> 1����
    // index = (x + (y * ���� ����)) * ����Ʈ ũ��
    
    int pitch{ BITMAP_WIDTH * BITMAP_BYTECOUNT };
    int index{ (x * BITMAP_BYTECOUNT) + (y * pitch) };

    float alpha = color.a;

    mspBackBuffer[index] = mspBackBuffer[index] * (1 - alpha) + static_cast<UINT8>(color.r * 255) * alpha;
    mspBackBuffer[index + 1] = mspBackBuffer[index+1] * (1 - alpha) + static_cast<UINT8>(color.g * 255) * alpha;
    mspBackBuffer[index + 2] = mspBackBuffer[index+2] * (1 - alpha) + static_cast<UINT8>(color.b * 255) * alpha;
    //mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);
}

void BitmapExample::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            DrawPixelToBuffer(left + x, top + y, color);
        }
    }
}
