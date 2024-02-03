#pragma once

namespace DX
{
	static class Utility
	{
	public:
		static ID3D11Texture2D* GetTextureFromSRV(ID3D11ShaderResourceView* pSRV);
		static void DebugString(LPCWSTR format, ...);
	};
}
