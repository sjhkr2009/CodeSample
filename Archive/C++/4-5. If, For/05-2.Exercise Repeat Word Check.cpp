#include<iostream>

int main()
{
	std::string currentInput{};
	std::string prevInput{};
	int count{ 1 };

	std::cout << "반복을 체크할 문장을 입력하세요."  << std::endl << 
				 " * 인덱스를 초기화하려면 _init을, 입력을 종료하려면 Ctrl + Z 를 입력하세요." << std::endl;

	while (std::cin >> currentInput) //string으로 값을 받을 수 있으면 true를 반환한다. 단, string은 어떤 입력을 받든 true가 되므로, 입력을 종료하려면 Ctrl + Z 를 입력해야 한다.
	{
		if (currentInput == "_init")
		{
			count = 1;
			std::cout << "인덱스가 초기화되었습니다." << std::endl;
			continue;
		}
		if (currentInput == prevInput)
		{
			std::cout << (count - 1) << "번째 단어와 " <<
						 count << "번째 단어 \'" << currentInput << "\' 가 반복되었습니다." << std::endl;
		}
		prevInput = currentInput;
		++count;
	}
}