#include "D3DFramework.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	D3DFramework instance;

	instance.Initialize(hInstance);

	instance.GameLoop();

	instance.Destroy();

	return 0;
}