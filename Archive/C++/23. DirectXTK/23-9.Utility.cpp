#include "PCH.h"
#include <array>

using Microsoft::WRL::ComPtr;
using namespace DX;

// ShaderResourceView 정보로부터 원본 이미지의 텍스쳐 정보를 추출하여 반환합니다.
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

// 디버그 모드에서만 동작합니다. 입력한 포맷에 따라 디버그 메시지를 출력합니다. C스타일의 printf 포맷을 사용합니다.
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
