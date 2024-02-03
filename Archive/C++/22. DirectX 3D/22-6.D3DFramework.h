#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>
#include "Timer.h"
#include "Input.h"

#pragma comment (lib, "d3d11.lib")

class D3DFramework
{
private:
	const std::wstring CLASS_NAME{ L"MyWindowClass" };
	const std::wstring TITLE_NAME{ L"Direct3D" };

protected:
	int mScreenWidth{ 800 };
	int mScreenHeight{ 600 };

	bool mMinimized{ false };
	bool mMaximized{ false };
	bool mResizing{ false };

	HWND		mHwnd{};
	HINSTANCE	mInstance{};

	// 입력 클래스
	MyUtility::Input	mInput{};
	MyUtility::Timer	mTimer{};
	std::wstring		mTitleText{};
	bool				mPaused{};

	Microsoft::WRL::ComPtr<ID3D11Device>		mspDevice{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	mspDeviceContext{};
	Microsoft::WRL::ComPtr<IDXGISwapChain>		mspSwapChain{};

	// Resource
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		mspRenderTarget{};
	Microsoft::WRL::ComPtr<ID3D11Texture2D>		mspDepthStencil{};

	// View
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mspRenderTargetView{};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mspDepthStencilView{};

private:
	void InitWindow(HINSTANCE hInstance);
	void InitD3D();

	void CalculateFPS();

protected:
	void OnResize();
	void RenderFrame();
	virtual void Render() {}
	virtual void Update(float delta) {};

public:
	virtual void Initialize(HINSTANCE hInstance, int width = 800, int height = 600);
	virtual void Destroy();
	void GameLoop();


public:
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
