#include <iostream>
#include "String.h"

// String Class
// 문자열 관리 기능 구현하기

// str1[0]			(예: char c = str1[2])
// str1[0] = 'a'	(예: "ZYX" -> "aYX")

// str1 + str2		(예: "a" + "b" = "ab")
// str1 = str2
// str1 += str2
// std::cout << str1

int main()
{
	String str1("abcd");
	String str2;
	String str3(10);
	String str4(str1);

	std::cout << str4[1] << std::endl;
	
	str1[1] = 'a';
	std::cout << str1 << ' ' << str4 << std::endl;
	
	String str5(str1 + str4);
	std::cout << str5[6] << std::endl;

	str2 += str1;
	str3 = str1;
	std::cout << str1 << '\t' << str2 << '\t' << str3 << std::endl;

	str2 += str4;
	for (int i = 0; i < str2.GetLength(); i++)
	{
		std::cout << str2[i];
	}
	std::cout << std::endl;
}
