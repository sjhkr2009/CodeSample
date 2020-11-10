#include <iostream>

int main()
{
	int array[10]{ 0,1,2,3,4,5,6,7,8,9 };

	int* ptr{ &array[0] };
	std::cout << ptr << " : " << *ptr << std::endl;

	ptr = nullptr;

	// 배열 자체를 포인터에 대입하면, 해당 배열의 첫 번째 값의 주소가 대입된다.
	// 배열은 내부적으로 숫자들이 담긴 주소를 가지고 있기 때문이다.
	ptr = array;
	std::cout << ptr << " : " << *ptr << std::endl;

	// 포인터의 타입을 int로 지정해줬기 때문에, 포인터에 1을 더하는 것은 int의 크기인 4바이트만큼 포인터를 이동하는 것을 의미한다.
	// 즉 이제 array[1] 의 주소를 가리킨다.
	ptr++;
	std::cout << ptr << " : " << *ptr << std::endl;

	int sum{};
	for (int i = 0; i < 10; i++)
	{
		// 실제로 이렇게 쓰지는 않지만, 배열은 포인터이므로 이런 식의 표기도 가능하다.
		sum += *(array + i); // (배열의 첫 번째 인덱스 주소 + i칸 이동한 주소)의 값을 가져와 sum에 더한다.
	}

	// 포인터 변수를 for문에 선언하여 다음과 같이 수정할 수 있다.
	for (int i = 0, *p = array; i < 10; ++i, ++p)
	{
		sum += *p;
	}



	char myString[]{ " is integer value." };
	int myInteger{ 5 };

	std::cout << myInteger + myString << std::endl;
	// 여기서 출력은 'nteger value.'가 된다.
	// myString은 해당 문자열의 첫 번째 주소값을 가진 포인터이며, char 타입은 1바이트의 크기를 갖는다.
	// 따라서 여기에 5를 더하면 myString이 가진 주소값에서 5바이트 이동한 칸부터 출력하므로, 앞의 5글자가 잘리고 출력된다.

	char myString2[]{ "는 정수입니다." };
	std::cout << myInteger + myString2 << std::endl;
	// 여기서 출력은 '수입니다.'가 된다. 한글은 한 글자당 2바이트이기 때문이다.

	// 다만 char은 1바이트 단위이므로, 한글 한 글자에서 1바이트만 잘라내면 이상한 유니코드 값이 나온다.
	std::cout << 4 + myString2 << std::endl;

	// 이를 방지하기 위해 아스키 기반인 char 문자열 대신 유니코드 기반인 wchar_t에 한국어를 지정하여 사용한다.
	// w는 wide의 약자로 유니코드 문자를 다룰 때 사용되며, 유니코드 문자열 앞에는 'L'(string Literal)을 붙인다.
	// 유니코드는 여러 국가의 언어를 표현하므로 인코딩이 필요한데, locale 타입에서 "kor"을 지정하면 한국어를 의미한다. 이를 통해 imbue(locale) 을 이용하면 유니코드 출력(wcout)의 인코딩 방식을 결정할 수 있다.
	// 이 방식은 나중에 다룰 예정
	wchar_t myString3[]{ L"는 정수입니다." };
	std::locale myLocale("kor");
	std::wcout.imbue(myLocale);
	std::wcout << myInteger + myString3 << std::endl;
}