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


// 함수를 컴파일할 때 C와 C++은 다른 형태로 만든다.
// C언어는 작성한 코드 그대로 컴파일되지만, C++은 다형성으로 인해 변수나 함수가 어디에 속하는지에 대한 정보가 필요하다.
// extern "C" 키워드는 C 스타일로 컴파일하라는 의미. 다형성 정보나 클래스 정보들이 불필요하다.
extern "C"
{
	// __declspec(ext-attr): 속성 선언(Declare Specification). 다음에 오는 선언자가 어떻게 동작할지 정보를 제공한다. extend-attribute에 키워드를 넣어 사용한다. (마이크로소프트 전용)
	// DLL Export: dll로 내보낸다. 그래픽카드 드라이버의 dll(보통 C형태로 작성됨)에 이 이름의 변수가 들어 있다는 의미.
	// 즉 그래픽카드 드라이버의 특정 옵션을 변경한다. (dll에 이 변수가 없으면 아무 동작도 하지 않음)
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;		// Nvidia 그래픽카드 드라이버에 들어 있는 옵티머스 기능이 이 프로그램에 항상 고성능의 GPU를 사용하도록 설정한다.
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;	// AMD 그래픽카드 드라이버에 들어 있는 Power X Press 기능이 이 프로그램에 항상 고성능의 GPU를 사용하도록 설정한다.

	// * Optimus / PowerXPress : GPU가 2개 이상일 경우 용도에 맞게 GPU를 전환하는 기능.
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