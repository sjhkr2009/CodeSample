#include "LinkedList.h"
#include <cassert>

ListNode* IntList::AddFirst(int value)
{
	ListNode* pElement = new ListNode{};
	pElement->data = value;

	if (IsEmpty())
	{
		pHead = pElement;
		pTail = pElement;
	}
	else
	{
		pHead->pPrev = pElement;
		pElement->pNext = pHead;
		pHead = pElement;
	}

	count++;
	return pElement;
}

ListNode* IntList::AddLast(int value)
{
	ListNode* pElement = new ListNode{};
	pElement->data = value;

	if (IsEmpty())
	{
		pHead = pElement;
		pTail = pElement;
	}
	else
	{
		pTail->pNext = pElement;
		pElement->pPrev = pTail;
		pTail = pElement;
	}
	
	count++;
	return pElement;
}

void IntList::PrintList()
{
	if (IsEmpty())
	{
		return;
	}

	ListNode* pElement{ pHead };

	do
	{
		std::cout << pElement->data << ' ';
		pElement = pElement->pNext;
	} while (pElement != nullptr);

	std::cout << std::endl;
}

int IntList::RemoveFirst()
{
	if (IsEmpty())
	{
		assert("Null Reference Exception : 빈 배열의 원소에 접근을 시도했습니다.");
		return 0;
	}

	ListNode* pElenemt{ pHead };
	int value = pElenemt->data;
	count--;

	if (IsEmpty())
	{
		pHead = pTail = nullptr;
	}
	else
	{
		pHead = pElenemt->pNext;
		pHead->pPrev = nullptr;
	}

	delete pElenemt;
	
	return value;
}

int IntList::RemoveLast()
{
	if (IsEmpty())
	{
		assert("Null Reference Exception : 빈 배열의 원소에 접근을 시도했습니다.");
		return 0;
	}
	
	ListNode* pElenemt{ pTail };
	int value = pElenemt->data;
	count--;

	if (IsEmpty())
	{
		pHead = pTail = nullptr;
	}
	else
	{
		pTail = pElenemt->pPrev;
		pTail->pNext = nullptr;
	}

	delete pElenemt;

	return value;
}

bool IntList::IsEmpty()
{
	return (count == 0);
}

void IntList::Clear()
{
	while (count > 0)
	{
		RemoveLast();
	}
	pHead = nullptr;
	pTail = nullptr;
	count = 0;
}
