/*
#include "PCH.h"
#include "Game.h"

using namespace DirectX;

namespace
{
	std::unique_ptr<Game> gName;
}

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
*/


// �Լ��� �������� �� C�� C++�� �ٸ� ���·� �����.
// C���� �ۼ��� �ڵ� �״�� �����ϵ�����, C++�� ���������� ���� ������ �Լ��� ��� ���ϴ����� ���� ������ �ʿ��ϴ�.
// extern "C" Ű����� C ��Ÿ�Ϸ� �������϶�� �ǹ�. ������ ������ Ŭ���� �������� ���ʿ��ϴ�.
extern "C"
{
	// __declspec(ext-attr): �Ӽ� ����(Declare Specification). ������ ���� �����ڰ� ��� �������� ������ �����Ѵ�. extend-attribute�� Ű���带 �־� ����Ѵ�. (����ũ�μ���Ʈ ����)
	// DLL Export: dll�� ��������. �׷���ī�� ����̹��� dll(���� C���·� �ۼ���)�� �� �̸��� ������ ��� �ִٴ� �ǹ�.
	// �� �׷���ī�� ����̹��� Ư�� �ɼ��� �����Ѵ�. (dll�� �� ������ ������ �ƹ� ���۵� ���� ����)
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;		// Nvidia �׷���ī�� ����̹��� ��� �ִ� ��Ƽ�ӽ� ����� �� ���α׷��� �׻� ������ GPU�� ����ϵ��� �����Ѵ�.
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;	// AMD �׷���ī�� ����̹��� ��� �ִ� Power X Press ����� �� ���α׷��� �׻� ������ GPU�� ����ϵ��� �����Ѵ�.

	// * Optimus / PowerXPress : GPU�� 2�� �̻��� ��� �뵵�� �°� GPU�� ��ȯ�ϴ� ���.
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{



	return 0;
}