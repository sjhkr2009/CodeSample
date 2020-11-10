#include <iostream>
#include "Calculator.h"

int main()
{
	try
	{
		Calculator calc{};
		calc.Start();
	}
	catch (std::exception e)
	{
		std::cerr << "오류가 발생했습니다 : " << e.what() << std::endl;
		return 1;
	}

	std::cout << "프로그램을 종료합니다." << std::endl;
}