#pragma once

#include "Monster.h"

struct MonsterList
{
	Monster* pHead{ nullptr };
	Monster* pTail{ nullptr };
};

Monster* CreateMonster(MonsterList& list, const char* name, const int hp);
int GetCountMonsterList(MonsterList& list);
void PrintMonsterList(MonsterList& list);
void PrintMonsterList(Monster* pMonster);
void PrintMonster(Monster* pMonster);
Monster* FindMonster(MonsterList& list, const char* name);
void DeleteList(MonsterList& list);
bool Delete(MonsterList& list, const char* name);