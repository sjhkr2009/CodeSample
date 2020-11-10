#include "Stack.h"
#include <iostream>

/*
	Direct X 사용 시, Init -> ...(사용) -> Release 의 과정을 거친다.
	즉 사용 전후에 Init와 Release를 수행한다. 이는 C++에서 new와 delete에 해당하는 메모리 할당 및 해제 절차이다.
*/

void Initialize(Stack& stack)
{
	stack.count = 0;
	stack.pTop = nullptr;
}

void ReleaseAll(Stack& stack)
{
	Element* pElem{ stack.pTop };
	Element* pNext{ nullptr };

	while (pElem != nullptr)
	{
		pNext = pElem->pNext;
		delete pElem;
		pElem = pNext;
	}

	Initialize(stack);
}

void Push(Stack& stack, int value)
{
	Element* pElem{ new Element{} };
	pElem->value = value;

	// 스택의 링크는 top에서 시작하여 Next를 통해 처음 원소까지 간다. top이 빠지면 그 Next가 top으로 지정되어야 하기 때문이다.
	pElem->pNext = stack.pTop;

	stack.pTop = pElem;
	stack.count++;
}

bool Pop(Stack& stack)
{
	if (stack.count == 0)
	{
		std::cout << "STACK IS EMPTY!!" << std::endl;
		return false;
	}
	
	Element* pPrevTop{ stack.pTop };
	int value = pPrevTop->value;

	stack.pTop = pPrevTop->pNext;
	stack.count--;

	delete pPrevTop;
	std::cout << "pop : " << value << std::endl;
}

void Print(Stack& stack)
{
	Element* pElem{ stack.pTop };

	while (pElem != nullptr)
	{
		std::cout << pElem->value << std::endl;
		pElem = pElem->pNext;
	}
}