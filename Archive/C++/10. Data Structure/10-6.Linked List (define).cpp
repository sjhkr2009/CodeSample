#include "LinkedList.h"
#include "DoubleLinkedList.h"

Monster* CreateMonster(MonsterList& list, const char* name, const int hp)
{
	Monster* element = new Monster();
	
	// 구조체 '포인터'이므로 elememt.Hp가 아니라 element->Hp 임에 유의한다.
	element->Hp = hp;

	// Monster에서 초기화하지 않았을 경우, 안전을 위해 포인터를 초기화해준다.
	element->pNext = nullptr;
	
	// name은 문자열 참조이므로, 값을 그대로 대입할 수 없다.
	// for문을 돌며 원소를 하나씩 대입하거나, strcpy 함수를 이용해야 한다. 매개변수는 (목적지, 문자열 길이, 원본)
	strcpy_s(element->Name, NAME_LENGTH, name);

	// 리스트의 마지막 요소(tail)가 존재할 경우 tail의 next를 새로 추가한 값으로 할당한다.
	if (list.pTail != nullptr)
	{
		list.pTail->pNext = element;
	}
	// tail이 없다면 리스트가 비어 있는 것이므로, pHead도 이 요소로 지정한다.
	else
	{
		list.pHead = element;
	}

	// 이 요소를 리스트의 tail로 지정한다.
	list.pTail = element;
	
	return element;
}

int GetCountMonsterList(MonsterList& list)
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

void PrintMonsterList(MonsterList& list)
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

Monster* FindMonster(MonsterList& list, const char* name)
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

void DeleteList(MonsterList& list)
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

bool Delete(MonsterList& list, const char* name)
{
	Monster* pElement{ list.pHead };
	Monster* pPrev{ nullptr };

	while (pElement != nullptr)
	{
		if (!strcmp(pElement->Name, name))
		{
			break;
		}
		pPrev = pElement;
		pElement = pElement->pNext;
	}

	if (pElement == nullptr)
	{
		std::cout << "----- Delete Failed! -----" << std::endl;
		return false;
	}

	if (pElement == list.pHead && pElement == list.pTail)
	{
		list.pHead = list.pTail = nullptr;
	}
	else if (pElement == list.pHead)
	{
		list.pHead = pElement->pNext;
	}
	else if (pElement == list.pTail)
	{
		list.pTail = pPrev;
		pPrev->pNext = nullptr;
	}
	else
	{
		pPrev->pNext = pElement->pNext;
	}

	std::cout << "----- " << pElement->Name << " Deleted! -----" << std::endl;
	delete pElement;

	return true;
}
