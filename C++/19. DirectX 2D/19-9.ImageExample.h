#pragma once
#include "D2DFramework.h"

// �̹��� ���� ���� ������ 4������ ������ �� �ִ�.
// 1. ���:           ������ � ��������, � ������ �����ϴ���.
// 2. ��Ʈ�� ����:    �����͵��� ����
// 3. �ȷ�Ʈ:         ���� �� ���� ǥ�������� ������ ������ ��(256��), �׸��� ����ϱ� ���� �ʿ��� ���� �ȷ�Ʈ�� �÷����� �׸��� �׷ȴ�. ����� ��� X
// 4. ��Ʈ�� ������:  ���� �ȼ����� �迭. (jpg ���� ���������� ������ �����ϴ� ������ �ʿ�������, ���⼭ �ǽ��� bmp ������ ���� ���� ������ �ʿ����� �ʴ�.)

// bmp������ �����쿡�� ��������Ƿ�, �̸� �б� ���� �ڵ���� ������API�� ����Ǿ� �ִ�.
// �� 1~3�� BITMAPFILEHEADER, BITMAPINFOHEADER, LOGPALETTE ����ü�� ������ �� �ְ�, ���� ������(4��)�� void �����ͷ� ���ϴ� �ȼ��� �о�� �� �ִ�.

// bmp ������ ��� �� ��Ʈ�� ������ ������ ����.
// bmp������ ó�� 2����Ʈ�� 'BM' ���� �����Ѵ�.
// BM ���� 4����Ʈ���� ������ ũ��(byte)�� ���� �ִ�.
//      �� ���� 2x2����Ʈ�� Ȥ�� ���� ������ ���� �� ��(������� ������ 0���� ����)
//      �� ���� 4����Ʈ�� ��Ʈ�� �����Ͱ� ó�� ��Ʈ�κ��� �� ����Ʈ �ں��� �����ϴ���(=Offset) ���� �ִ�.
//      �� ���� 4x2����Ʈ�� ������ �ʺ�� ����
//      �� ���� 2����Ʈ�� ���� Ȯ���� ���� plane��(����� ������ �ʾ� 1�� ����)
//      �� ���� 2����Ʈ�� �� ��Ʈ �̹������� ���� �ִ�.

// ��Ʈ�� ���� ����
/*
    1. BMP ����
    2. ���� ����� ��Ʈ�� ���� ���� (BITMAPFILEHEADER, BITMAPINFOHEADER ����ü ���)
    3. FILEHEADER�� offset��ŭ �̵�
    4. �ȼ� ������ �б� (��Ʈ�� ���� ��� 'BITMAPINFOHEADER'�� bitCount)
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

