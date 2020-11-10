#include <Windows.h>

// ������ API

// ����ũ�μ���Ʈ���� ��������Ƿ�, �ñ��� ��Ҵ� F1�� ���� ������ ���� ���� ��õ.

int WINAPI WinMain(
	// Handle of Instance : �������� exe������ OOP�� �������� ������ ��ü, Ŭ������ �ش��Ѵ�. HINSTANCE�� �����Ų ������ �ǹ��Ѵ�.
	_In_ HINSTANCE hInstance,
	//						���α׷� ������ �ٸ� ���α׷��� ������ ���, ���� ���α׷��� �� ���� ���´�. 
	_In_opt_ HINSTANCE hPrevInstance,
	// Long Pointer to Null-Terminated String
	// exe ������ ���� Ŭ������ �ʰ� ��� ������Ʈ�� ���� ������ ��, ���� â�� ��� �� ��(�⺻/�ּ�ȭ/�ִ�ȭ ��) ������ �� �ִ�.
	//		ex) ���� ��ɾ� �ڿ� -windows 1920x1080 (â ��� 1920x1080���� ����)���� �ٿ� �ɼ��� ������ �� �ִ�.
	_In_ LPSTR lpCmdLine,
	// exe ������ ���� Ŭ������ �ʰ� �ٷ� ���⸦ ���� ������ ��, ���� â�� ��� �� ��(�⺻/�ּ�ȭ/�ִ�ȭ ��) ������ �� �ִ�.
	//		ex) ���� ��ġ �ڿ� -windows 1920x1080 (â ��� 1920x1080���� ����)���� �ٿ� �ɼ��� ������ �� �ִ�.
	_In_ int nShowCmd
)
{
	// MessageBox : ��ȭ ���� (���̾�α� �ڽ�) ����
	// �Ű������� ��ȭâ�� ����� �̸��� LPCWSTR �������� �޴´�. ������ ���ڸ� ���� ��ȭ ���� ���� �����ܰ� ��ư�� ������ �� �ִ�.
	//		�� LPCWSTR: Long Pointer to Constant Wide String. ��� ������ ���̵� ��Ʈ��.
	MessageBox(nullptr, L"Hello World", L"SimpleWindow1", MB_ICONEXCLAMATION | MB_OK);

	// ��ȭ ���ڿ��� ��ȣ�ۿ� ����� int�� ���� �� �ִ�.
	int result = MessageBox(nullptr, L"Hello World", L"SimpleWindow2", MB_ICONEXCLAMATION | MB_YESNO);

	// ����� ���� ó���� �� �� �ִ�.
	if (result == IDYES)
	{
		OutputDebugString(L"����!");
	}
	else if (result == IDNO)
	{
		OutputDebugString(L"����");
	}
	
	return 0;
}