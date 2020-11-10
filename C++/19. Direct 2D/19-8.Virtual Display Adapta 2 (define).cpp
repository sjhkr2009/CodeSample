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

    //FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Red);
    //FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(D2D1::ColorF::LightGreen, 0.5f));

    DrawLineToBuffer(3, 3, 3, 10, D2D1::ColorF::Black);

    DrawCircleToBuffer(320, 320, 300, D2D1::ColorF::Red);

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
    // CopyFromMemory: 메모리를 복사해 온다. 매개변수는 (복사할 크기, 복사할 메모리 포인터, 가로축 하나의 메모리 크기)
    //                  * 복사할 크기에 nullptr을 입력하면 전체를 복사해 온다.
    mspFrameBitmap->CopyFromMemory(nullptr, mspBackBuffer.get(), BITMAP_WIDTH * BITMAP_BYTECOUNT);
}

void BitmapExample::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
    // 2차원 -> 1차원
    // index = (x + (y * 가로 길이)) * 바이트 크기
    
    int pitch{ BITMAP_WIDTH * BITMAP_BYTECOUNT };
    int index{ (x * BITMAP_BYTECOUNT) + (y * pitch) };

    float alpha = color.a;
    float inverseAlpha = 1.0f - alpha;

    UINT8 RGBInt[3]{
        static_cast<UINT8>(color.r * 255),
        static_cast<UINT8>(color.g * 255),
        static_cast<UINT8>(color.b * 255)
    };
    
    // 알파 블렌딩 (Alpha Blending)
    // (기존 색상 * 투명도) + (입력된 색상 * 불투명도)를 곱한다. 참고로 이는 수많은 알파 블렌딩 방식 중 하나.
    mspBackBuffer[index]        = (mspBackBuffer[index] * inverseAlpha) + (RGBInt[0] * alpha);
    mspBackBuffer[index + 1]    = (mspBackBuffer[index+1] * inverseAlpha) + (RGBInt[1] * alpha);
    mspBackBuffer[index + 2]    = (mspBackBuffer[index+2] * inverseAlpha) + (RGBInt[2] * alpha);

    // 생략해도 출력은 같지만, 투명도 저장을 위해 color.a를 0~255로 변경하여 넣어준다.
    mspBackBuffer[index + 3] = static_cast<UINT8>(color.a * 255);
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

void BitmapExample::DrawLineToBuffer(int startX, int startY, int endX, int endY, D2D1::ColorF color)
{
    if (endX - startX < 0)
    {
        int temp = startX;
        startX = endX;
        endX = temp;
    }
    if (endY - startY < 0)
    {
        int temp = startY;
        startY = endY;
        endY = temp;
    }
    
    int intervalX = (endX - startX);
    int intervalY = (endY - startY);
    if (intervalX == 0 && intervalY == 0)
    {
        return;
    }

    bool drawByX = (intervalX > intervalX);
    float gradient = drawByX ?  (float)(endY - startY) / (endX - startX) : (float)(endX - startX) / (endY - startY);
    
    if (drawByX)
    {
        for (int nowX = startX; nowX <= endX; nowX++)
        {
            int nowY = (gradient * (nowX - startX)) + startY;
            DrawPixelToBuffer(nowX, nowY, color);
        }
    }
    else
    {
        for (int nowY = startY; nowY <= endY; nowY++)
        {
            int nowX = (gradient * (nowY - startY)) + startX;
            DrawPixelToBuffer(nowX, nowY, color);
        }
    }
}

void BitmapExample::DrawLineToBuffer_Reference(int startX, int startY, int endX, int endY, D2D1::ColorF color)
{
    int dx{ endX - startX };
    int dy{ endY - startY };

    if (dx == 0 && dy == 0)
    {
        return;
    }

    int sign{};

    if (abs(dx) > abs(dy))
    {
        sign = dx < 0 ? -1 : 1;
        int y{};
        for (int x = startX; x != endX; x += sign)
        {
            y = static_cast<int>((dy / dx) * (x - startX) + startY);
            DrawPixelToBuffer(x, y, color);
        }
    }
    else
    {
        sign = dy < 0 ? -1 : 1;
        int x{};
        for (int y = startY; y != endY; y += sign)
        {
            x = static_cast<int>((dx / dy) * (y - startY) + startX);
            DrawPixelToBuffer(x, y, color);
        }
    }
}

void BitmapExample::DrawCircleToBuffer(int originX, int originY, int radius, D2D1::ColorF color)
{
    // 참고: 호도법에 의한 원 공식
    // x = cos(θ) * r
    // y = sin(θ) * r
    
    if (originY < radius || originX < radius)
    {
        return;
    }

    const double pi{ 3.141592653589793 };
    double theta;
    double precision{ (double)1 / radius };

    for (theta = 0; theta < 2 * pi; theta += precision)
    {
        DrawPixelToBuffer(
            originX + (cos(theta) * radius),
            originY + (sin(theta) * radius),
            color
        );
    }
}
