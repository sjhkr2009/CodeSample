
#include "DoubleLinkedList.h"

Monster* CreateMonster(MonsterDoubleList& list, const char* name, const int hp)
{
	Monster* element = new Monster();

	element->Hp = hp;

	element->pNext = nullptr;

	strcpy_s(element->Name, NAME_LENGTH, name);

	if (list.pTail != nullptr)
	{
		list.pTail->pNext = element;
		element->pPrev = list.pTail;
	}
	else
	{
		list.pHead = element;
	}

	list.pTail = element;

	return element;
}

int GetCountMonsterList(MonsterDoubleList& list)
{
	int count{ 0 };
	Monster* pElement{ list.pHead };

	while (pElement != nullptr)
	{
		pElement = pElement->pNext;
		count++;
	}

	return count;
}

void PrintMonsterList(MonsterDoubleList& list)
{
	if (list.pHead == nullptr)
	{
		std::cout << "List is Empty!" << std::endl;
		return;
	}
	PrintMonsterList(list.pHead);
}

void PrintMonster(Monster* pMonster)
{
	std::cout << "{ " << pMonster->Name << ", " << pMonster->Hp << " }" << std::endl;
}

void PrintMonsterList(Monster* pMonster)
{
	if (pMonster == nullptr)
		return;

	PrintMonster(pMonster);
	PrintMonsterList(pMonster->pNext);
}

Monster* FindMonster(MonsterDoubleList& list, const char* name)
{
	Monster* pMonster{ list.pHead };

	while (pMonster != nullptr)
	{
		if (!strcmp(pMonster->Name, name))
		{
			return pMonster;
		}
		pMonster = pMonster->pNext;
	}

	return nullptr;
}

void DeleteList(MonsterDoubleList& list)
{
	Monster* pCurrent{ list.pHead };
	Monster* pNext{ nullptr };

	while (pCurrent != nullptr)
	{
		pNext = pCurrent->pNext;
		delete pCurrent;
		pCurrent = pNext;
	}

	list.pHead = nullptr;
	list.pTail = nullptr;
}

bool Delete(MonsterDoubleList& list, const char* name)
{
	Monster* pElement{ list.pHead };

	while (pElement != nullptr)
	{
		if (!strcmp(pElement->Name, name))
		{
			break;
		}
		pElement = pElement->pNext;
	}

	if (pElement == nullptr)
	{
		std::cout << "----- Delete Failed! (on Double Linked List) -----" << std::endl;
		return false;
	}

	if (pElement == list.pHead && pElement == list.pTail)
	{
		list.pHead = list.pTail = nullptr;
	}
	else if (pElement == list.pHead)
	{
		list.pHead = pElement->pNext;
		list.pHead->pPrev = nullptr;
	}
	else if (pElement == list.pTail)
	{
		list.pTail = pElement->pPrev;
		list.pTail->pNext = nullptr;
	}
	else
	{
		pElement->pPrev->pNext = pElement->pNext;
		pElement->pNext->pPrev = pElement->pPrev;
	}

	std::cout << "----- " << pElement->Name << " Deleted! (on Double Linked List) -----" << std::endl;
	delete pElement;

	return true;
}
