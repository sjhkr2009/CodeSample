#include <iostream>
#include <regex>

// 정규표현식 (Regular Expression) - include regex 필요
// 자료에서 특정 패턴을 찾아내거나, 특정 패턴으로 수정할 때 사용한다.

// 이메일이나 아이디 등 사용자 입력의 유효성을 검증하거나, XML/Json 등 특정 형식의 파일을 읽어올 때,
// 또는 이벤트 당첨자 발표 때 아이디 일부를 가리는 등 특정 형태로 변환할 때 사용한다.

int main()
{
	std::string email;
	std::cin >> email;

	std::regex requiredPattern("[0-9a-zA-Z]([-_.]?[0-9a-zA-Z])*@[0-9a-zA-Z]([-_.]?[0-9a-zA-Z])*.[a-zA-Z]{2,3}");
	//혹시 이스케이프 시퀀스 때문에 타 언어의 정규표현식 사용에 문제가 생길 경우, 원시 문자열 리터럴을 사용하면 된다. (13-9 코드 참고)

	if (std::regex_match(email, requiredPattern))
	{
		std::cout << "GOOD" << std::endl;
	}
	else
	{
		std::cout << "WRONG" << std::endl;
	}

	// 정규표현식의 문법에 대해서는 공부하거나, 대중적인 요소(이메일 형식 등)는 검색을 통해 알아낸 후 검증해보고 사용한다.
}