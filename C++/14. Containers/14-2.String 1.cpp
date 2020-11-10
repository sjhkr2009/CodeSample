#include <iostream>
#include <string> //include string이 없어도 str::string 자체는 쓸 수 있지만, 내부 기능 일부는 이용할 수 없다. (to_string 등)

// string: 문자열을 담은 컨테이너

void Print(std::string text)
{
	std::cout << text << std::endl;
}
void Print(char text)
{
	std::cout << text << std::endl;
}

int main()
{
	std::string str1;
	std::string str2{ "abcd" };
	std::string str3{ "1234" };

	str1 = str2;
	Print(str1);

	str1 += str3;
	Print(str1);

	str1 = str1 + str2;
	Print(str1);

	str1[2] = 'O';
	Print(str1);

	std::string str4{ str1 == str2 };
	Print(str4);

	const char* str5;
	str5 = str1.c_str();
	Print(str5);

	std::string::iterator itr = str1.begin();
	Print(itr[2]);

	int a{ 4256 };
	float b{};
	str1 = std::to_string(a);
	Print(str1);

	a = std::stoi(str3);
	b = std::stof(str3);
	std::cout << a << ' ' << b << std::endl;
}
