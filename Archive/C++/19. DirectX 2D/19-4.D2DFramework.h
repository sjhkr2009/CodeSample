#pragma once

// ������ ���α׷��ֿ��� Direct 2D ��� �� ����ϴ� �����ӿ�ũ

// D2D Factory ���� �� �ʱ�ȭ: 
// Render Target ����

#include <d2d1.h>
#include <wrl.h>

class D2DFramework
{
	// ����: ����� ���� �����ڰ� ���ų�, �����ڿ��� ���� �ʱ�ȭ���� �ʴ� ��� ������ �ʱ�ȭ���� ����Ѵ�.

protected:
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspD2DRenderTarget{};

public:
	virtual HRESULT	Initialize(HWND hwnd);
	virtual void	Release();
	virtual void	Render();

private:
	void ShowErrorMsg(HRESULT errCode, LPCWSTR errMsg, LPCWSTR errTitle = L"Error");
};

