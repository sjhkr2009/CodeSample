#include <iostream>
#include <string>
#include <bitset>
#include <sstream>


int main()
{
	//wstring
	wchar_t c1 = L'ㄱ';
	std::wstring str1 = L"한글한글";

	std::wcout << str1 << std::endl;

	// 출력 형식 변환
	int value{ 1234 };
	float value2{ 3.141592f };
	bool boolean{ true };

	std::cout << std::boolalpha << boolean << std::endl; //bool을 알파벳으로 출력
	std::cout << std::dec << value << '\t' << std::hex << value << std::endl; //10진수 또는 16진수로 출력
	std::cout << std::bitset<6>(value) << std::endl; // 이진수로 출력 (6자리까지)

	std::cout.precision(3); //자릿수 제한
	std::cout << value2 << std::endl; //3자리인 3.14까지만 출력됨

	std::cout.width(10); // 자릿수 지정
	std::cout << std::dec << value << std::endl; // 10자리 수로 출력되며, 남는 곳은 빈칸. (위의 std::hex는 다시 10진수로 초기화해준다)

	std::cout.fill('0'); // 빈 공간에 출력할 내용 지정
	std::cout.width(10);
	std::cout << value << std::endl; // 10자리 수로 출력되며, 남는 곳에는 0이 들어감.

	// string stream (#include <sstream> 필요)
	std::ostringstream oss;
	oss << "정수 " << value << ", " << "실수 " << value2 << std::endl;

	std::string str2 = oss.str();
	std::cout << str2 << std::endl;
}
