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

	bool isExit{ pRight == pLeft };
	bool isOverFirst{ false };
	bool isOverLast{ false };
	if (isExit)
	{
		return;
	}

	do
	{
		while (pRight->value > pivot)
		{
			if (pRight == pLeft)
			{
				isExit = true;
			}
			if (pRight == pFirst)
			{
				isOverFirst = true;
			}
			pRight = pRight->pPrev;
		}

		while (pLeft->value < pivot)
		{
			if (pLeft == pRight)
			{
				isExit = true;
			}
			if (pLeft == pLast)
			{
				isOverLast = true;
			}
			pLeft = pLeft->pNext;
		}
		
		if (!isExit)
		{
			_swapValue(pLeft, pRight);
			if (pRight == pFirst)
			{
				isOverFirst = true;
			}
			if (pLeft == pLast)
			{
				isOverLast = true;
			}
			pLeft = pLeft->pNext;
			if (pLeft == pRight || pLeft == pRight->pNext)
			{
				isExit = true;
			}
			pRight = pRight->pPrev;
		}


	} while (!isExit);

	if (!isOverLast && pLeft != nullptr)
	{
		_qSortRecursive(queue, pLeft, pLast);
	}
	if (!isOverFirst && pRight != nullptr)
	{
		_qSortRecursive(queue, pFirst, pRight);
	}
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
