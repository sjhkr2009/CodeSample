#include<iostream>

int main()
{
	std::string a;
	std::string b;

	std::cout << "\"문자\"를 입력하세요." << std::endl;

	std::cin >> a >> b;
	std::cout << std::endl << "입력한 문자는 다음과 같습니다." << std::endl << a << std::endl << b;
;
}