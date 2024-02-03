#include <iostream>

// 프로그램 실행 시 메모리 구조
//	1. 스택(stack) : 지역 변수나 함수가 할당되는 공간
//	2. 힙(Heap) : 동적 메모리 할당 시 사용되는 공간
//	3. 데이터(Data) : 전역 변수가 할당되는 공간

void MyFunc1();
void MyFunc2();

char gString[]{ "MEMORY LAYOUT" }; // (1) Data에 저장됨
int gInteger; // (2) Data에 저장됨

int main() // (3) Stack에 저장됨
{
	int* pAlloc = new int; // (4) heap에 저장됨

	MyFunc1(); // (5) Stack에 저장됨

	delete pAlloc; // (11) (4)가 Heap에서 사라짐
	pAlloc = nullptr;
}

void MyFunc1() // (5) Stack에 저장됨
{
	int local; // (6) Stack에 저장됨
	MyFunc2(); // (7) Stack에 저장됨
} // (10) (5),(6)이 Stack에서 지워짐(Pop)

void MyFunc2() // (7) Stack에 저장됨
{
	int local; // (8) Stack에 저장됨
} // (9) (7),(8)이 Stack에서 지워짐(Pop)


// 스택은 저장공간이 작다.(Visual Studio 기준 1MB) -> 큰 데이터를 쓰면 안 된다.
// 배열을 int array[10000000] 과 같이 선언하면 stack에 저장되는데, 이렇게 되면 배열을 몇 번만 사용해도 stack overflow가 발생한다.
// 이런 에러는 포착하기 매우 어렵다. 한두 개의 실행에는 문제가 없지만 이러한 할당이 몇 번 일어나면 스택 공간이 가득차기 때문에, 개별 코드에서는 에러가 안 나지만 협업 결과 에러가 난다.
// 따라서 배열 크기를 알고 있고 고정값을 갖더라도, 어느정도 공간이 크다면 new를 사용하여 힙에 할당한다.