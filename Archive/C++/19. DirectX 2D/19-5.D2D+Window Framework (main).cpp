#include <Windows.h>
#include "D2DFramework.h"

// ���� ���ӿ��� �����ӿ�ũ�� ��ӹ��� �� ������ �����ϴ� ������ ����Ѵ�.
class Tetris : public D2DFramework { };

// �����ӿ�ũ �׽�Ʈ�� ��ü ���� (�ʱ�ȭ�� ������ ���� �Ŀ�)
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