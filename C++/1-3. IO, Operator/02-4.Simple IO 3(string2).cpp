#include<iostream>

int main()
{
	std::string name{};
	unsigned int age{};

	std::cout << "이름과 나이를 입력하고 엔터를 누르세요." << std::endl;

	std::cin >> name >> age;

	std::cout << "안녕하십니까? " 
		<< name 
		<< "(" 
		<< age 
		<< ")" 
		<< " 님!" 
		<< std::endl;
}