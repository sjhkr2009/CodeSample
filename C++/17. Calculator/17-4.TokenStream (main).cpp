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
		std::cerr << "������ �߻��߽��ϴ� : " << e.what() << std::endl;
		return 1;
	}

	std::cout << "���α׷��� �����մϴ�." << std::endl;
}