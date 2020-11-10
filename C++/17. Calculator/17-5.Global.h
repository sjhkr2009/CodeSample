#pragma once
#include <iostream>

void ThrowInputErr(const char* errorText)
{
	std::cin.clear();
	throw std::logic_error(errorText);
}

namespace Commands
{
	const char QUIT{ 'q' };
	const char QUIT_SUB{ 'Q' };

	const char RESET{ '0' };

	const char CALC_END{ ';' };
	const char CALC_END_SUB{ '=' };
}

namespace Constants
{
	const std::string PROMPT{ "> " };
	const std::string RESULT{ "= " };
}

namespace Message
{
	namespace Common
	{
		const char MENUAL[]{ "계산식을 입력하고 ; 또는 = 을 입력하세요. (Q를 누르면 종료합니다)" };
		const char RESET_CHECK[]{ "종료하려면 0을, 아니라면 아무 키나 입력하세요." };
		const char RESET[]{ "계산기를 초기화합니다..." };
		const char QUIT[]{ "프로그램을 종료합니다." };
	}

	namespace Parenthasis
	{
		const char ERROR[]{ "(오류)" };
		const char TOKEN_ERROR[]{ "(예상하지 못 한 토큰)" };
		const char RUNTIME_ERROR[]{ "(실행 중 오류 발생)" };
		const char PRIMARY_ERROR[]{ "(규칙 위반 기호)" };
	}

	namespace Error
	{
		const char TOKEN_INVALID[]{ "잘못된 토큰입니다." };
		const char TOKEN_OVERLAP[]{ "버퍼에 이미 토큰이 있습니다." };
		const char DEVIDE_BY_ZERO[]{ "0으로 나눌 수 없습니다." };
		const char PARENTHASIS_OPENED[]{ "\')\' 기호가 감지되지 않습니다. 괄호를 닫지 않았는지 확인해주세요." };
		const char PRIMARY_INVALID[]{ "수식이 규칙에 맞지 않습니다." };
	}
}