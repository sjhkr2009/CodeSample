#include <iostream>


// 클래스 내에도 열거형, 구조체, 다른 클래스를 선언할 수 있다.
// 열거형 외에는 실제로 잘 활용되지는 않는다.

class Cat
{
public:
	enum Color
	{
		WHITE = 0xFFFFFFFF,
		BLACK = 0xFF000000
	};

	struct Element
	{
		int value;
	};

	class SubClass
	{
		int v1;
	};

public:
	Color mColor;

	void PrintColor() { std::cout << mColor; }
};

int main()
{
	Cat cat{};

	Cat::Color color = Cat::Color::WHITE;
	cat.mColor = color;

	cat.PrintColor();
}