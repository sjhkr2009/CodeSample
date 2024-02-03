#pragma once

#include "Monster.h"

struct MonsterDoubleList
{
	Monster* pHead{ nullptr };
	Monster* pTail{ nullptr };
};

Monster* CreateMonster(MonsterDoubleList& list, const char* name, const int hp);
int GetCountMonsterList(MonsterDoubleList& list);
void PrintMonsterList(MonsterDoubleList& list);
void PrintMonsterList(Monster* pMonster);
void PrintMonster(Monster* pMonster);
Monster* FindMonster(MonsterDoubleList& list, const char* name);
void DeleteList(MonsterDoubleList& list);
bool Delete(MonsterDoubleList& list, const char* name);