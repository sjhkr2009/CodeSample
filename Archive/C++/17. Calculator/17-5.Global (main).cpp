#include "Global.h"
#include "Calculator.h"

int main()
{
	while (true)
	{
		try
		{
			Calculator calc{};
			calc.Start();
			break;
		}
		catch (std::exception e)
		{
			std::cerr << Message::Parenthasis::ERROR << e.what() << std::endl;

			char input;
			std::cout << Message::Common::RESET_CHECK << std::endl << "> ";
			std::cin >> input;
			if (input == Commands::RESET)
			{
				std::cout << Message::Common::QUIT << Message::Parenthasis::RUNTIME_ERROR << std::endl;
				return 1;
			}
			else
			{
				std::cin.clear();
				std::cout << Message::Common::RESET << std::endl;
				continue;
			}
		}
	}

	std::cout << Message::Common::QUIT << std::endl;
}