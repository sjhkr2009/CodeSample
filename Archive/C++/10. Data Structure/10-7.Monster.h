#pragma once

#include <iostream>

const int NAME_LENGTH{ 16 };

struct Monster
{
	char Name[NAME_LENGTH]{};
	int Hp{};

	Monster* pPrev{ nullptr };
	Monster* pNext{ nullptr };
};