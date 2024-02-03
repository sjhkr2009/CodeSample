#include <iostream>

// 입력받은 문자를 대문자로 변경하는 함수
char ConvertToUpperCase(char input);

int main()
{
    char buffer[50] = "Hello world";

    for (int i = 0; i < 50; i++)
    {
        std::cout << ConvertToUpperCase(buffer[i]);
    }
}

char ConvertToUpperCase(char input)
{
    if (input < 'a' || input > 'z') return input;

    return input + ('A' - 'a');
}
