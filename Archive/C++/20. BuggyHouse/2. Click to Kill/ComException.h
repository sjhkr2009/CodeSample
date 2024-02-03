#pragma once

#include <Windows.h>
#include <exception>
#include <sstream>

// COM 객체에 대한 예외 클래스
class com_exception : public std::exception
{
public:
	com_exception(HRESULT hr) :						result{ hr }, description{} {}
	com_exception(HRESULT hr, std::string msg) :	result{ hr }, description{ msg } {}

	virtual const char* what() const noexcept override
	{
		std::ostringstream oss;
		oss << "Failure with HRESULT of " << std::hex << result;

		if (!description.empty())
		{
			oss << std::endl << ": " << description;
		}

		return oss.str().c_str();
	}

private:
	HRESULT result;
	std::string description;
};

// COM 객체 생성 여부 확인
// 헤더에 전역 함수의 정의가 들어 있는 경우, 여러 번 재정의되는 에러가 발생할 수 있다. 이를 방지하기 위해서 inline을 붙이는 것을 권장.
inline void ThrowIfFailed(HRESULT hr, std::string msg = "")
{
	if (FAILED(hr))
	{
		throw com_exception(hr, msg);
	}
}

