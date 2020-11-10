#include "DoubleLinkedList.h"

int main()
{
	MonsterDoubleList list;

	CreateMonster(list, "Wolf", 50);
	CreateMonster(list, "Sheep", 15);
	CreateMonster(list, "Bear", 75);

	PrintMonsterList(list);
	std::cout << FindMonster(list, "Sheep")->Name << std::endl;

	Delete(list, "Sheep");
	PrintMonsterList(list);
	Delete(list, "Bear");
	PrintMonsterList(list);
	Delete(list, "Tiger");
	PrintMonsterList(list);
	Delete(list, "Wolf");
	PrintMonsterList(list);
}