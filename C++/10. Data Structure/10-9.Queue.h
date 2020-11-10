#pragma once

enum Command
{
	ENQUEUE = 1,
	DEQUEUE = 2,
	SORT = 3,
	EXIT = 9
};

struct Element
{
	int value{};
	Element* pNext{ nullptr };
	Element* pPrev{ nullptr };
};

struct Queue
{
	Element* pFirst{};
	Element* pLast{};
	int count{};
};

void Initialize(Queue& queue);
void ReleaseAll(Queue& queue);
void Enqueue(Queue& queue, int value);
bool Dequeue(Queue& queue);
void QSort(Queue& queue);
void _qSortRecursive(Queue& queue, Element* pFirst, Element* pLast);
void _swapValue(Element* pOne, Element* pOther);
void Print(Queue& queue);