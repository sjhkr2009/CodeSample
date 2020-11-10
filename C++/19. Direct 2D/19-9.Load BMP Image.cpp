#include "ImageExample.h"
#include <fstream>
#include <vector>

HRESULT ImageExample::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	return LoadBMP(L"Data/32.bmp", mspBitmap.GetAddressOf());
}

void ImageExample::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	mspRenderTarget->DrawBitmap(mspBitmap.Get());

	HRESULT hr = mspRenderTarget->EndDraw();

	if (hr == D2DERR_RECREATE_TARGET)
	{
		CreateDeviceResources();
	}
}

HRESULT ImageExample::LoadBMP(LPCWSTR filename, ID2D1Bitmap** ppBitmap)
{
// 1. ��Ʈ�� ����
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

// 2. BITMAPFILEHEADER ����ü
	BITMAPFILEHEADER fileHeader;
	file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

// 3. BITMAPINFOHEADER ����ü
	BITMAPINFOHEADER infoHeader;
	file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));


	// ������ ��Ʈ�� Ÿ���� �ƴϰų�, 32��Ʈ�� �ƴ� ��� ������ ��ȯ�Ѵ�.
	// �޸𸮿��� ��Ʋ ����� ������� �о���Ƿ� Ÿ���� 0x424D���� ������ �ٲ�� 0x4D42�� �о�ȿ� ����.
	if ((fileHeader.bfType != 0x4D42) || (infoHeader.biBitCount != 32))
	{
		return E_FAIL;
	}

// 4. FILEHEADER�� Offset(=���� �ȼ� �����Ͱ� �� ����Ʈ �ڿ� �ִ°�) ��ŭ �ڷ� �̵�

	// ������ ó������ n����Ʈ �ǳʶپ� Ž���� ���� ifstream.seekg(n) �� ����Ѵ�. (Seek Global)
	file.seekg(fileHeader.bfOffBits);

	// �ȼ� �����͸� ���� �迭�� �����. 1����Ʈ �����̹Ƿ� char Ÿ���̸�, biSizeImage���� ���Ұ� �� ������ �����.
	std::vector<char> pPixels(infoHeader.biSizeImage);

// 5. �ȼ� ������ (INFOHEADER�� bitCount)

	// pitch: �������� ����Ʈ ũ��. �ȼ� ������ 1�ȼ��� ����Ʈ�� ���ؼ� ���Ѵ�.
	// biBitCount�� �ȼ� ũ�⸦ ��Ʈ ������ ��ȯ�ϹǷ� 8�� ������ ����Ʈ�� �ٲ��ش�.
	int pixelSize = infoHeader.biBitCount / 8;
	int width = infoHeader.biWidth;
	int pitch = width * pixelSize;
	int height = infoHeader.biHeight;


	// �ȼ� �����ͷ� �̵������Ƿ�, �ȼ� �������� ������ŭ �迭�� �����͸� �о�´�.
	// �о�� ������ �����ͷδ�, ���� ������ ������ ù ��° ������ �ּҸ� �ѱ��.
	//file.read(&pPixels[0], infoHeader.biSizeImage);

	// but. ��Ʈ�� �̹����� �ȼ� �����Ͱ� �Ųٷ� �����Ǿ� �־, �̷��� ��ü�� ������ �̹����� �Ųٷ� ������.
	// ���� 1�پ� �о ������ ������ �κк��� �������� ä����� �Ѵ�.
	/*
	for (int y = height - 1; y >= 0; y--)
	{
		int index = pitch * y;
		file.read(&pPixels[index], pitch);
	}
	*/

	char bgColor[3]{ static_cast<char>(30), static_cast <char>(199), static_cast <char>(250) };
	// ���⼭ �߰��� ������ �����ϰ� �����Ѵ�.
	// �� ���� �� �پ� �д� ��� �� �ϳ��� �о, ���� ������ �����ϸ� ���İ��� 0���� ó���ؾ� �Ѵ�.
	/*for (int y = height - 1; y >= 0; y--)
	{
		int index = pitch * y;
		for (int x = 0; x < pitch; x += pixelSize)
		{
			file.read(&pPixels[index + x], pixelSize);
			bool isAlpha = true;
			for (int i = 0; i < 3; i++)
			{
				if (static_cast<unsigned char>(pPixels[index + x + i]) != bgColor[i])
				{
					isAlpha = false;
					break;
				}
			}
		}
	}*/
	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			char b, g, r, a;

			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (b == bgColor[2] && g == bgColor[1] && r == bgColor[0])
			{
				r = g = b = a = 0;
			}

			pPixels[(x * pixelSize) + (y * pitch)] = b;
			pPixels[(x * pixelSize) + (y * pitch) + 1] = g;
			pPixels[(x * pixelSize) + (y * pitch) + 2] = r;
			pPixels[(x * pixelSize) + (y * pitch) + 3] = a;
		}
	}


// 6. ���� �ݱ�
	file.close();

	// ��Ʈ�� ��ü�� �����Ѵ�.
	
	// ���� Ÿ���� ���� ��Ʈ�� �����Ͱ� ����Ű�� ��ü�� �ʱ�ȭ�Ѵ�. ũ��� ����, ��Ʈ�� �������� �����͸� �ѱ��.
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(infoHeader.biWidth, infoHeader.biHeight),
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		mspBitmap.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// ��Ʈ�� �����Ͱ� ����Ű�� ��ü�� �ȼ� ������ ����

	mspBitmap->CopyFromMemory(
		nullptr,		// nullptr�̸� ��ü�� �о�´�.
		&pPixels[0],
		pitch
	);

	return S_OK;
}
