#pragma once

// �̸� �����ϵ� ��� (Pre-Compiled Header) - ���� ���� ��
// - ���� ���� �� ������Ʈ �Ӽ� - C/C++ - �̸� �����ϵ� ������� �������ϼ���.
// - ��� cpp���Ͽ��� �� ����� include �Ǿ�� �մϴ�.

#include <winsdkver.h>	// Windows SDK Version
#define _WIN32_WINNT 0x0A00
// winsdkver.h�� ���ǵǾ� �ִ� ������ �Է��Ͽ�, ���� ����� �� � �������� ����Ǿ�� �ϴ��� ������ �� �ִ�.
// ���⼭�� ������ 10 ������ �����ϵ��� �����.
// win7 : 0x0601
// win8 : 0x0602
// win10 : 0x0A00

#include <sdkddkver.h>	// SDK DDK(Driver Development Kit) Version

// Windows API�� min/max�� �����Ѵ�.
// ������API�� STL�� ��� min/max ����� �־ �ߺ����� ���� ������ ������ �߻��ϱ� ������ �����Ѵ�.
#define NOMINMAX

// ������ �ӵ� ��� �� �뷮 ��Ҹ� ���� ��ġ��
// WIN32_LEAN_AND_MEAN�� ���Ե� ��ɵ� �� ������� ���� ��ҵ��� �����Ѵ�.
#define NODRWTEXT
#define NOGDI
#define NOBITMAP

#define NOMCX		//	(MCX: Model Configuration Extension, �� ���� ���)
#define NOSERVICE	// nProtect �� ���� ������ ���
#define NOHELP		// F1 ���� �׸�

#define WIN32_LEAN_AND_MEAN


// ���� ���Ǵ� ��� ���ϵ�
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

#ifdef _DEBUG	// ����� ��忡���� Ȱ��ȭ�ȴ�. (release�� ���� ��Ȱ��ȭ)
#include <dxgidebug.h>	// HAL�� ������� �����Ƿ�, 
#endif

#pragma warning(push)			// ��� ����
#pragma warning(disable:26812)	// enum�� enum class�� ��ü�϶�� ��� �����մϴ�.

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

#pragma warning(pop)			// ��� ���� ����

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
			// ���� ����Ʈ�� ���۸� �޸𸮿� ���� �÷ȴ� ������ �ݺ��ϸ� �޸� ����ȭ(Fragmentation)�� �߻��ϹǷ� static���� �����Ѵ�.
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

// ���� �ĺ���. ����� ��������� ����ϴ� ������ ���̰� ����.
// DXGUID(Globally Unique Identifier):	�����쿡�� ����ϴ� ���� �ĺ���
// UUID(Universally Unique Identifier): ������ �� ȯ��(Ư�� ����/��Ʈ��ũ)���� �ַ� ����ϴ� ���� �ĺ���

// pragma�� ���̺귯���� �Է��ϴ� ���, [������Ʈ �Ӽ� - ��Ŀ - �Է� - �߰� ���Ӽ�] ���� ���̺귯�� �̸��� �Է��� �� �ִ�.
// �� ��, ����� ������ �÷����� ��� ����/��� �÷������� ������ ��.
//#pragma comment (lib, "dxguid.lib")
