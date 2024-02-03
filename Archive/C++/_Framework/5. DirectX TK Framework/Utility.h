#pragma once

// Utility: 게임 전반적으로 사용할 유용한 기능들의 집합입니다.

namespace DX
{
	class Utility
	{
	public:
		static ID3D11Texture2D* GetTextureFromSRV(ID3D11ShaderResourceView* pSRV);
		static void DebugString(LPCWSTR format, ...);
	};
}
