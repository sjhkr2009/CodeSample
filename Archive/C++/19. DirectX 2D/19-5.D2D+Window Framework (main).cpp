#include <Windows.h>
#include "D2DFramework.h"

// 만들 게임에서 프레임워크를 상속받은 후 본문을 구성하는 식으로 사용한다.
class Tetris : public D2DFramework { };

// 프레임워크 테스트용 객체 선언 (초기화는 윈도우 생성 후에)
Tetris myFramework;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	int ret;

	try
	{
		myFramework.Initialize(hInstance);

		ret = myFramework.GameLoop();

		myFramework.Release();
	}
	catch (const com_exception& e)
	{
		MessageBoxA(nullptr, e.what(), "DirectX Exception", MB_OK);
	}

	return ret;
}