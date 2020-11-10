#pragma once

// 윈도우 프로그래밍에서 Direct 2D 사용 시 사용하는 프레임워크

// D2D Factory 생성 및 초기화: 
// Render Target 생성

#include <d2d1.h>
#include <wrl.h>
#include <exception>
#include <sstream>

// COM 객체에 대한 예외 클래스
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

// COM 객체 생성 여부 확인
// 헤더에 전역 함수의 정의가 들어 있는 경우, 여러 번 재정의되는 에러가 발생할 수 있다. 이를 방지하기 위해서 inline을 붙이는 것을 권장.
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
}

class D2DFramework
{
	// 참고: 사용자 정의 생성자가 없거나, 생성자에서 값을 초기화하지 않는 경우 유니폼 초기화식을 사용한다.
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
	// 콜백 함수는 인스턴스 없이도 운영체제가 호출할 수 있도록, public 정적 함수로 생성해야 한다.
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

