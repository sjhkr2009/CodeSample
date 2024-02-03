#pragma once

// ������ ���α׷��ֿ��� Direct 2D ��� �� ����ϴ� �����ӿ�ũ

// D2D Factory ���� �� �ʱ�ȭ: 
// Render Target ����

#include <d2d1.h>
#include <wrl/client.h>
#include "ComException.h"

class D2DFramework
{
	// ����: ����� ���� �����ڰ� ���ų�, �����ڿ��� ���� �ʱ�ȭ���� �ʴ� ��� ������ �ʱ�ȭ���� ����Ѵ�.
private:
	const wchar_t* WindowClassName{ L"MyWindowClass" };

protected:
	HWND mHwnd{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT	InitD2D(HWND hwnd);
	virtual HRESULT CreateDeviceResources();

public:
	// �ݹ� �Լ��� �ν��Ͻ� ���̵� �ü���� ȣ���� �� �ֵ���, public ���� �Լ��� �����ؾ� �Ѵ�.
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual HRESULT	Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768);
	virtual void	Release();
	virtual int		GameLoop();
	virtual void	Render();

	ID2D1HwndRenderTarget* GetRenderTarget() const { return mspRenderTarget.Get(); }

private:
	void ErrorBox(LPCWSTR errorText) { MessageBox(nullptr, errorText, L"Error", MB_OK); }
};

