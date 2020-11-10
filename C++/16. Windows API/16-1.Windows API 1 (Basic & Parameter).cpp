#include <Windows.h>

// 윈도우 API

// 마이크로소프트에서 만들었으므로, 궁금한 요소는 F1을 눌러 도움말을 보는 것을 추천.

int WINAPI WinMain(
	// Handle of Instance : 실행중인 exe파일은 OOP의 관점에서 각각의 객체, 클래스에 해당한다. HINSTANCE는 실행시킨 파일을 의미한다.
	_In_ HINSTANCE hInstance,
	//						프로그램 내에서 다른 프로그램을 실행할 경우, 이전 프로그램에 이 값이 들어온다. 
	_In_opt_ HINSTANCE hPrevInstance,
	// Long Pointer to Null-Terminated String
	// exe 파일을 직접 클릭하지 않고 명령 프롬프트를 통해 실행할 때, 실행 창을 어떻게 할 지(기본/최소화/최대화 등) 설정할 수 있다.
	//		ex) 실행 명령어 뒤에 -windows 1920x1080 (창 모드 1920x1080으로 실행)등을 붙여 옵션을 지정할 수 있다.
	_In_ LPSTR lpCmdLine,
	// exe 파일을 직접 클릭하지 않고 바로 가기를 통해 실행할 때, 실행 창을 어떻게 할 지(기본/최소화/최대화 등) 설정할 수 있다.
	//		ex) 시작 위치 뒤에 -windows 1920x1080 (창 모드 1920x1080으로 실행)등을 붙여 옵션을 지정할 수 있다.
	_In_ int nShowCmd
)
{
	// MessageBox : 대화 상자 (다이얼로그 박스) 띄우기
	// 매개변수로 대화창의 내용과 이름을 LPCWSTR 형식으로 받는다. 마지막 인자를 통해 대화 상자 안의 아이콘과 버튼을 지정할 수 있다.
	//		ㄴ LPCWSTR: Long Pointer to Constant Wide String. 상수 형태의 와이드 스트링.
	MessageBox(nullptr, L"Hello World", L"SimpleWindow1", MB_ICONEXCLAMATION | MB_OK);

	// 대화 상자와의 상호작용 결과를 int로 받을 수 있다.
	int result = MessageBox(nullptr, L"Hello World", L"SimpleWindow2", MB_ICONEXCLAMATION | MB_YESNO);

	// 결과에 따른 처리를 할 수 있다.
	if (result == IDYES)
	{
		OutputDebugString(L"ㅇㅇ!");
	}
	else if (result == IDNO)
	{
		OutputDebugString(L"ㄴㄴ");
	}
	
	return 0;
}