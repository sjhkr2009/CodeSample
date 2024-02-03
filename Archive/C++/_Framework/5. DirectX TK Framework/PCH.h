#pragma once

// 미리 컴파일된 헤더 (Pre-Compiled Header) - 설정 오류 시
// - 설정 오류 시 프로젝트 속성 - C/C++ - 미리 컴파일된 헤더에서 재지정하세요.
// - 모든 cpp파일에는 이 헤더가 include 되어야 합니다.

#include <winsdkver.h>	// Windows SDK Version
#define _WIN32_WINNT 0x0A00
// winsdkver.h에 정의되어 있는 버전을 입력하여, 앱이 실행될 때 어떤 버전에서 실행되어야 하는지 정의할 수 있다.
// 여기서는 윈도우 10 에서만 동작하도록 만든다.
// win7 : 0x0601
// win8 : 0x0602
// win10 : 0x0A00

#include <sdkddkver.h>	// SDK DDK(Driver Development Kit) Version

// Windows API의 min/max를 제거한다.
// 윈도우API와 STL에 모두 min/max 기능이 있어서 중복으로 인해 컴파일 에러가 발생하기 때문에 지정한다.
#define NOMINMAX

// 컴파일 속도 향상 및 용량 축소를 위한 조치들
// WIN32_LEAN_AND_MEAN에 포함된 기능들 중 사용하지 않을 요소들을 제거한다.
#define NODRWTEXT
#define NOGDI
#define NOBITMAP

#define NOMCX		//	(MCX: Model Configuration Extension, 모뎀 지원 기능)
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

#include <list>
#include <map>
#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>

#ifdef _DEBUG	// 디버그 모드에서만 활성화된다. (release일 때는 비활성화)
#include <dxgidebug.h>	// HAL은 디버깅이 어려우므로, 
#endif

#pragma warning(push)			// 경고 무시
#pragma warning(disable:26812)	// enum을 enum class로 대체하라는 경고를 무시합니다.

// DirectX TK
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

// RapidJson
#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:26451)
#pragma warning(disable:26495)
#pragma warning(disable:6319)
#pragma warning(disable:6386)
#pragma warning(disable:6385)
#include "rapidjson/document.h"
#pragma warning(pop)


//----------------------------------------------------------------------
// Custom Headers

#include "Utility.h"
#include "StepTimer.h"
#include "DeviceResources.h"

#include "TextureManager.h"
#include "JsonManager.h"

#include "Actor.h"
#include "AnimatedActor.h"
#include "ActorManager.h"

#include "GameBase.h"

//----------------------------------------------------------------------

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

// 고유 식별자. 비슷한 기능이지만 사용하는 진영에 차이가 있음.
// DXGUID(Globally Unique Identifier):	윈도우에서 사용하는 고유 식별자
// UUID(Universally Unique Identifier): 윈도우 외 환경(특히 서버/네트워크)에서 주로 사용하는 고유 식별자

// pragma로 라이브러리를 입력하는 대신, [프로젝트 속성 - 링커 - 입력 - 추가 종속성] 에서 라이브러리 이름을 입력할 수 있다.
// 이 때, 상단의 구성과 플랫폼을 모든 구성/모든 플랫폼으로 설정할 것.
//#pragma comment (lib, "dxguid.lib")
