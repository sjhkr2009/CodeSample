#include "Stack.h"

Stack::Stack() :
	mCount{ 0 },
	mpTop{ nullptr }
{
}

Stack::~Stack()
{
	Node* pElem{ mpTop };
	Node* pNext{ nullptr };

	while (pElem != nullptr)
	{
		pNext = pElem->mpNext;
		delete pElem;
		pElem = pNext;
	}
	
	mCount = 0; // 굳이 필요하진 않으나, 생성한 요소 그대로 초기화하는 것이 안전한 코딩 방식이다.
	mpTop = nullptr;
}

void Stack::Push(int value)
{
	Node* pNew = new Node(value);

	pNew->mpNext = mpTop;
	mpTop = pNew;
	mCount++;
}

bool Stack::Pop()
{
	if (mCount == 0)
	{
		std::cout << "STACK IS EMPTY!!" << std::endl;
		return false;
	}
	
	Node* pPrevTop = mpTop;
	int value = pPrevTop->mData;

	mpTop = pPrevTop->mpNext;
	mCount--;
	delete pPrevTop;

	std::cout << "pop : " << value << std::endl;
	return true;
}

void Stack::Print()
{
	Node* pElem{ mpTop };

	while (pElem != nullptr)
	{
		std::cout << pElem->mData << std::endl;
		pElem = pElem->mpNext;
	}
}
