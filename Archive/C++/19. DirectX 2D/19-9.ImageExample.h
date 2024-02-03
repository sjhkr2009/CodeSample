#pragma once
#include "D2DFramework.h"

// 이미지 파일 내의 정보는 4가지로 구분할 수 있다.
// 1. 헤더:           파일이 어떤 형태인지, 어떤 포맷을 지원하는지.
// 2. 비트맵 정보:    데이터들의 형식
// 3. 팔레트:         과거 한 번에 표현가능한 색상이 적었을 때(256색), 그림을 출력하기 위해 필요한 색상만 팔레트에 올려놓고 그림을 그렸다. 현재는 사용 X
// 4. 비트맵 데이터:  실제 픽셀들의 배열. (jpg 등의 압축파일은 압축을 해제하는 과정이 필요하지만, 여기서 실습할 bmp 파일은 압축 해제 과정이 필요하지 않다.)

// bmp파일은 윈도우에서 만들었으므로, 이를 읽기 위한 코드들이 윈도우API에 내장되어 있다.
// 위 1~3은 BITMAPFILEHEADER, BITMAPINFOHEADER, LOGPALETTE 구조체로 가져올 수 있고, 실제 데이터(4번)는 void 포인터로 원하는 픽셀을 읽어올 수 있다.

// bmp 파일의 헤더 및 비트맵 정보는 다음과 같다.
// bmp파일의 처음 2바이트는 'BM' 으로 시작한다.
// BM 뒤의 4바이트에는 파일의 크기(byte)가 나와 있다.
//      그 뒤의 2x2바이트는 혹시 몰라서 만들어둔 공간 두 개(사용하지 않으며 0으로 고정)
//      그 뒤의 4바이트는 비트맵 데이터가 처음 비트로부터 몇 바이트 뒤부터 시작하는지(=Offset) 적혀 있다.
//      그 뒤의 4x2바이트는 파일의 너비와 높이
//      그 뒤의 2바이트는 추후 확장을 위한 plane값(현재는 사용되지 않아 1로 고정)
//      그 뒤의 2바이트는 몇 비트 이미지인지 적혀 있다.

// 비트맵 파일 열기
/*
    1. BMP 열기
    2. 파일 헤더와 비트맵 정보 열기 (BITMAPFILEHEADER, BITMAPINFOHEADER 구조체 사용)
    3. FILEHEADER의 offset만큼 이동
    4. 픽셀 데이터 읽기 (비트맵 정보 헤더 'BITMAPINFOHEADER'의 bitCount)
*/

class ImageExample : public D2DFramework
{
private:
    Microsoft::WRL::ComPtr<ID2D1Bitmap> mspBitmap{};

public:
    virtual HRESULT	Initialize(HINSTANCE hInstance,
        LPCWSTR title = L"Direct2D Example",
        UINT width = 1024,
        UINT height = 768
    ) override;
    virtual void Render() override;

    HRESULT LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap);
};

