#include "MyHeader.h"

// extern을 붙이지 않으면, SourceA와 SourceB에 같은 함수 내용이 붙여넣기된다.
// 이미 int로 특수화된 MyFunction은 다시 붙여넣기하지 않도록 해 두면 최적화에 도움이 된다.
// 이 때 extern template을 사용한다.

extern template void MyFunction<int>();

void Func2()
{
	MyFunction<int>();
}