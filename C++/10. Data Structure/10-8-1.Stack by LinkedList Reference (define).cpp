#include "Stack.h"
#include <iostream>

/*
	Direct X ��� ��, Init -> ...(���) -> Release �� ������ ��ģ��.
	�� ��� ���Ŀ� Init�� Release�� �����Ѵ�. �̴� C++���� new�� delete�� �ش��ϴ� �޸� �Ҵ� �� ���� �����̴�.
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

	// ������ ��ũ�� top���� �����Ͽ� Next�� ���� ó�� ���ұ��� ����. top�� ������ �� Next�� top���� �����Ǿ�� �ϱ� �����̴�.
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