#include "MyHeader.h"

// extern�� ������ ������, SourceA�� SourceB�� ���� �Լ� ������ �ٿ��ֱ�ȴ�.
// �̹� int�� Ư��ȭ�� MyFunction�� �ٽ� �ٿ��ֱ����� �ʵ��� �� �θ� ����ȭ�� ������ �ȴ�.
// �� �� extern template�� ����Ѵ�.

extern template void MyFunction<int>();

void Func2()
{
	MyFunction<int>();
}