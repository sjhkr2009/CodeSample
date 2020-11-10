#include <sstream>
#include "D2DFramework.h"

#pragma comment (lib, "d2d1.lib")

HRESULT D2DFramework::Initialize(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, mspD2DFactory.GetAddressOf());
	if (FAILED(hr))
	{
		ShowErrorMsg(hr, L"Failed to Create Direct2D Factory");
		return hr;
	}

	RECT wr;
	GetClientRect(hwnd, &wr);	// 클라이언트 영역을 구해서 wr에 배정하는 함수.
	
	hr = mspD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hwnd,
			D2D1::SizeU(wr.right - wr.left, wr.bottom - wr.top)
		),
		mspD2DRenderTarget.GetAddressOf()
	);
	if (FAILED(hr))
	{
		ShowErrorMsg(hr, L"Failed to Create D2D HWND Render Target!");
		return hr;
	}

	return S_OK;
}

void D2DFramework::Release()
{
	mspD2DRenderTarget.Reset();
	mspD2DFactory.Reset();
}

void D2DFramework::Render()
{
	mspD2DRenderTarget->BeginDraw();
	mspD2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f));

	// TODO: Draw

	mspD2DRenderTarget->EndDraw();
}

void D2DFramework::ShowErrorMsg(HRESULT errCode, LPCWSTR errMsg, LPCWSTR errTitle)
{
	// 출력을 위한(output) 유니코드(wide) 스트링스트림 = wostringstream
	std::wostringstream oss;

	oss << "Error: 0x" << std::hex << errCode << std::endl;
	oss << errMsg;

	// 스트링스트림을 wstring으로 변경 후 다시 C스타일의 w_char* 로 변경
	MessageBox(nullptr, oss.str().c_str(), errTitle, MB_OK);
}
