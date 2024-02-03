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
// 1. 비트맵 열기
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

// 2. BITMAPFILEHEADER 구조체
	BITMAPFILEHEADER fileHeader;
	file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

// 3. BITMAPINFOHEADER 구조체
	BITMAPINFOHEADER infoHeader;
	file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));


	// 파일이 비트맵 타입이 아니거나, 32비트가 아닐 경우 에러를 반환한다.
	// 메모리에서 리틀 엔디안 방식으로 읽어오므로 타입이 0x424D에서 순서가 바뀌어 0x4D42로 읽어옴에 유의.
	if ((fileHeader.bfType != 0x4D42) || (infoHeader.biBitCount != 32))
	{
		return E_FAIL;
	}

// 4. FILEHEADER의 Offset(=실제 픽셀 데이터가 몇 바이트 뒤에 있는가) 만큼 뒤로 이동

	// 파일을 처음부터 n바이트 건너뛰어 탐색할 때는 ifstream.seekg(n) 을 사용한다. (Seek Global)
	file.seekg(fileHeader.bfOffBits);

	// 픽셀 데이터를 담을 배열을 만든다. 1바이트 단위이므로 char 타입이며, biSizeImage개의 원소가 들어갈 공간을 만든다.
	std::vector<char> pPixels(infoHeader.biSizeImage);

// 5. 픽셀 데이터 (INFOHEADER의 bitCount)

	// pitch: 가로축의 바이트 크기. 픽셀 개수에 1픽셀당 바이트를 곱해서 구한다.
	// biBitCount는 픽셀 크기를 비트 단위로 반환하므로 8로 나눠서 바이트로 바꿔준다.
	int pixelSize = infoHeader.biBitCount / 8;
	int width = infoHeader.biWidth;
	int pitch = width * pixelSize;
	int height = infoHeader.biHeight;


	// 픽셀 데이터로 이동했으므로, 픽셀 데이터의 개수만큼 배열에 데이터를 읽어온다.
	// 읽어올 공간의 포인터로는, 위에 선언한 벡터의 첫 번째 원소의 주소를 넘긴다.
	//file.read(&pPixels[0], infoHeader.biSizeImage);

	// but. 비트맵 이미지는 픽셀 데이터가 거꾸로 나열되어 있어서, 이렇게 전체를 읽으면 이미지가 거꾸로 찍힌다.
	// 따라서 1줄씩 읽어서 벡터의 마지막 부분부터 역순으로 채워줘야 한다.
	/*
	for (int y = height - 1; y >= 0; y--)
	{
		int index = pitch * y;
		file.read(&pPixels[index], pitch);
	}
	*/

	char bgColor[3]{ static_cast<char>(30), static_cast <char>(199), static_cast <char>(250) };
	// 여기서 추가로 배경색을 투명하게 변경한다.
	// 이 때는 한 줄씩 읽는 대신 점 하나씩 읽어서, 점이 배경색과 동일하면 알파값을 0으로 처리해야 한다.
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


// 6. 파일 닫기
	file.close();

	// 비트맵 객체에 저장한다.
	
	// 렌더 타겟을 통해 비트맵 포인터가 가리키는 객체를 초기화한다. 크기와 포맷, 비트맵 포인터의 포인터를 넘긴다.
	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(infoHeader.biWidth, infoHeader.biHeight),
		D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
		mspBitmap.GetAddressOf()
	);
	ThrowIfFailed(hr);

	// 비트맵 포인터가 가리키는 객체에 픽셀 데이터 저장

	mspBitmap->CopyFromMemory(
		nullptr,		// nullptr이면 전체를 읽어온다.
		&pPixels[0],
		pitch
	);

	return S_OK;
}
