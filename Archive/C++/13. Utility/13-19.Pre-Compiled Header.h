#pragma once

// 미리 컴파일된 헤더 (Pre-Compiled Header)
// 1. 프로젝트 우클릭 후 [속성 - C/C++ - 미리 컴파일된 헤더] 에서 지정한다.
// 2. 모든 cpp파일에 해당 헤더를 include한다.

#include <winsdkver.h>	// Windows SDK Version
#define _WIN32_WINNT 0x0A00
// winsdkver.h에 정의되어 있는 버전을 입력하여, 앱이 실행될 때 어떤 버전에서 실행되어야 하는지 정의할 수 있다.
// 여기서는 윈도우 10 에서만 동작하도록 만든다.
// win7 : 0x0601
// win8 : 0x0602
// win10 : 0x0A00

#include <sdkddkver.h>	// SDK DDK(Driver Development Kit, 개발 도구) Version

#define NOMINMAX
// min/max를 사용하지 않는다는 의미. Windows API의 min/max를 제거한다. (STL만 사용)
// 윈도우API와 STL에 모두 min/max 기능이 있어서 중복으로 인해 컴파일 에러가 발생하기 때문에 지정한다.

// 컴파일 속도 향상 및 용량 축소를 위한 조치들
// 게임에서 사용하지 않으면서도 WIN32_LEAN_AND_MEAN에 포함된 기능들을 제거한다.
#define NODRWTEXT
#define NOGDI
#define NOBITMAP

// 현재는 모뎀을 쓰는 곳이 거의 없으니 제외
#define NOMCX	//	(MCX: Model Configuration Extension, 모뎀 지원 기능)

#define NOSERVICE	// nProtect 등 게임 외적인 요소
#define NOHELP		// F1 도움말 항목

#define WIN32_LEAN_AND_MEAN


// 자주 사용되는 헤더 파일들
#include <Windows.h>
#include <wrl/client.h>

#include <d3d11_1.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

#ifdef _DEBUG	// 디버그 모드에서만 활성화된다. (release일 때는 비활성화)
#include <dxgidebug.h>	// HAL은 디버깅이 어려우므로, 
#endif

#pragma warning(push)			// 경고 무시
#pragma warning(disable:26812)	// 무시할 경고 번호

// 다이렉트XTK 헤더들
#include "Audio.h"
#include "CommonStates.h"
#include "WICTextureLoader.h"
#include "Effects.h"

#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"

#include "SimpleMath.h"

#pragma warning(pop)			// 경고 무시 종료

namespace DX
{
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) noexcept : result(hr) {}

		const char* what() const override
		{
			// 작은 바이트의 버퍼를 메모리에 자주 올렸다 내리길 반복하면 메모리 단편화(Fragmentation)가 발생하므로 static으로 선언한다.
			static char s_str[64]{};
			sprintf_s(s_str, "Failed with HRESULT of %08X", static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}
}