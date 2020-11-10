#pragma once

// ������ ���α׷��ֿ��� Direct 2D ��� �� ����ϴ� �����ӿ�ũ

// D2D Factory ���� �� �ʱ�ȭ: 
// Render Target ����

#include <d2d1.h>
#include <wrl.h>
#include <exception>
#include <sstream>

// COM ��ü�� ���� ���� Ŭ����
class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr) : result{ hr } {}
	virtual const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << "Failure with HRESULT of " << std::hex << result;
		return oss.str().c_str();
	}

private:
	HRESULT result;
};

// COM ��ü ���� ���� Ȯ��
// ����� ���� �Լ��� ���ǰ� ��� �ִ� ���, ���� �� �����ǵǴ� ������ �߻��� �� �ִ�. �̸� �����ϱ� ���ؼ� inline�� ���̴� ���� ����.
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
	// ����: ����� ���� �����ڰ� ���ų�, �����ڿ��� ���� �ʱ�ȭ���� �ʴ� ��� ������ �ʱ�ȭ���� ����Ѵ�.
private:
	const wchar_t* WindowClassName{ L"MyWindowClass" };

protected:
	HWND mHwnd{};
	Microsoft::WRL::ComPtr<ID2D1Factory> mspD2DFactory{};
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> mspD2DRenderTarget{};

protected:
	HRESULT InitWindow(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height);
	virtual HRESULT	InitD2D(HWND hwnd);

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

private:
	void ErrorBox(LPCWSTR errorText) { MessageBox(nullptr, errorText, L"Error", MB_OK); }
};

