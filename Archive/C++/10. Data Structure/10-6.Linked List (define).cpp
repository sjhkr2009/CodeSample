#include "LinkedList.h"
#include "DoubleLinkedList.h"

Monster* CreateMonster(MonsterList& list, const char* name, const int hp)
{
	Monster* element = new Monster();
	
	// ����ü '������'�̹Ƿ� elememt.Hp�� �ƴ϶� element->Hp �ӿ� �����Ѵ�.
	element->Hp = hp;

	// Monster���� �ʱ�ȭ���� �ʾ��� ���, ������ ���� �����͸� �ʱ�ȭ���ش�.
	element->pNext = nullptr;
	
	// name�� ���ڿ� �����̹Ƿ�, ���� �״�� ������ �� ����.
	// for���� ���� ���Ҹ� �ϳ��� �����ϰų�, strcpy �Լ��� �̿��ؾ� �Ѵ�. �Ű������� (������, ���ڿ� ����, ����)
	strcpy_s(element->Name, NAME_LENGTH, name);

	// ����Ʈ�� ������ ���(tail)�� ������ ��� tail�� next�� ���� �߰��� ������ �Ҵ��Ѵ�.
	if (list.pTail != nullptr)
	{
		list.pTail->pNext = element;
	}
	// tail�� ���ٸ� ����Ʈ�� ��� �ִ� ���̹Ƿ�, pHead�� �� ��ҷ� �����Ѵ�.
	else
	{
		list.pHead = element;
	}

	// �� ��Ҹ� ����Ʈ�� tail�� �����Ѵ�.
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
