#pragma once

#include <Windows.h>
#include <exception>
#include <sstream>

// COM ��ü�� ���� ���� Ŭ����
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

// COM ��ü ���� ���� Ȯ��
// ����� ���� �Լ��� ���ǰ� ��� �ִ� ���, ���� �� �����ǵǴ� ������ �߻��� �� �ִ�. �̸� �����ϱ� ���ؼ� inline�� ���̴� ���� ����.
inline void ThrowIfFailed(HRESULT hr, std::string msg = "")
{
	if (FAILED(hr))
	{
		throw com_exception(hr, msg);
	}
}

