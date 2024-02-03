#include "Queue.h"
#include <iostream>

void Initialize(Queue& queue)
{
	queue.count = 0;
	queue.pFirst = nullptr;
	queue.pLast = nullptr;
}

void ReleaseAll(Queue& queue)
{
	Element* pElem{ queue.pFirst };
	Element* pNext{ nullptr };

	while (pElem != nullptr)
	{
		pNext = pElem->pNext;
		delete pElem;
		pElem = pNext;
	}
}

void Enqueue(Queue& queue, int value)
{
	Element* pNew{ new Element{} };
	pNew->value = value;

	if (queue.count == 0)
	{
		queue.pFirst = pNew;
		queue.pLast = pNew;
		queue.count++;
		return;
	}

	queue.pLast->pNext = pNew;
	pNew->pPrev = queue.pLast;

	queue.pLast = pNew;
	queue.count++;
}

bool Dequeue(Queue& queue)
{
	if (queue.count == 0)
	{
		std::cout << "QUEUE IS EMPTY!!" << std::endl;
		return false;
	}

	Element* pPop{ queue.pFirst };
	std::cout << pPop->value << " is poped." << std::endl;

	if (queue.count == 1)
	{
		delete pPop;
		Initialize(queue);
		return true;
	}

	queue.pFirst = pPop->pNext;
	queue.pFirst->pPrev = nullptr;

	delete pPop;
	queue.count--;
	return true;
}

void QSort(Queue& queue)
{
	_qSortRecursive(queue, queue.pFirst, queue.pLast);
}

void _qSortRecursive(Queue& queue, Element* pFirst, Element* pLast)
{
	int pivot{ pFirst->value };
	Element* pRight{ pLast };
	Element* pLeft{ pFirst };

	if (pFirst == nullptr || pLast == nullptr || pFirst == pLast)
	{
		return;
	}

	do
	{
		while (pLeft->value < pivot)
		{
			pLeft = pLeft->pNext;
		}
		while (pRight->value > pivot)
		{
			pRight = pRight->pPrev;
		}
		if (pLeft->pPrev != pRight)
		{
			_swapValue(pLeft, pRight);
			pLeft = pLeft->pNext;
			pRight = pRight->pPrev;
		}
	} while (pLeft != nullptr && pRight != nullptr && pLeft->pPrev != pRight);

	// null 포인터를 넘기거나 원소가 1개일 때는 맨 위에서 예외처리하고, 여기서는 그냥 넘겨준다.
	_qSortRecursive(queue, pFirst, pRight);
	_qSortRecursive(queue, pLeft, pLast);
}

void _swapValue(Element* pOne, Element* pOther)
{
	int temp{ pOne->value };
	pOne->value = pOther->value;
	pOther->value = temp;
}

void Print(Queue& queue)
{
	Element* pElem{ queue.pFirst };

	while (pElem != nullptr)
	{
		std::cout << pElem->value << " ";
		pElem = pElem->pNext;
	}
	std::cout << std::endl;
}
