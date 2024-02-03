#include "PCH.h"
#include <array>

using Microsoft::WRL::ComPtr;
using namespace DX;

// ShaderResourceView �����κ��� ���� �̹����� �ؽ��� ������ �����Ͽ� ��ȯ�մϴ�.
ID3D11Texture2D* Utility::GetTextureFromSRV(ID3D11ShaderResourceView* pSRV)
{
	assert(pSRV != nullptr);

	ComPtr<ID3D11Resource> resource;
	pSRV->GetResource(resource.GetAddressOf());
	assert(resource != nullptr);

	ComPtr<ID3D11Texture2D> texture;
	resource->QueryInterface<ID3D11Texture2D>(texture.GetAddressOf());
	assert(texture != nullptr);

	return texture.Get();
}

// ����� ��忡���� �����մϴ�. �Է��� ���˿� ���� ����� �޽����� ����մϴ�. C��Ÿ���� printf ������ ����մϴ�.
void Utility::DebugString(LPCWSTR format, ...)
{
#ifdef _DEBUG
	static std::array<wchar_t, 1024> msg;
	msg.fill('\0');

	va_list va;
	va_start(va, format);

	int size = std::vswprintf(msg.data(), msg.size(), format, va);
	if (size < 1024)
	{
		msg.at(size) = L'\n';
	}
	OutputDebugStringW(msg.data());

	va_end(va);
#endif
}
