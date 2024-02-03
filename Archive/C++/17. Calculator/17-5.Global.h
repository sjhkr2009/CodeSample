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
		const char MENUAL[]{ "������ �Է��ϰ� ; �Ǵ� = �� �Է��ϼ���. (Q�� ������ �����մϴ�)" };
		const char RESET_CHECK[]{ "�����Ϸ��� 0��, �ƴ϶�� �ƹ� Ű�� �Է��ϼ���." };
		const char RESET[]{ "���⸦ �ʱ�ȭ�մϴ�..." };
		const char QUIT[]{ "���α׷��� �����մϴ�." };
	}

	namespace Parenthasis
	{
		const char ERROR[]{ "(����)" };
		const char TOKEN_ERROR[]{ "(�������� �� �� ��ū)" };
		const char RUNTIME_ERROR[]{ "(���� �� ���� �߻�)" };
		const char PRIMARY_ERROR[]{ "(��Ģ ���� ��ȣ)" };
	}

	namespace Error
	{
		const char TOKEN_INVALID[]{ "�߸��� ��ū�Դϴ�." };
		const char TOKEN_OVERLAP[]{ "���ۿ� �̹� ��ū�� �ֽ��ϴ�." };
		const char DEVIDE_BY_ZERO[]{ "0���� ���� �� �����ϴ�." };
		const char PARENTHASIS_OPENED[]{ "\')\' ��ȣ�� �������� �ʽ��ϴ�. ��ȣ�� ���� �ʾҴ��� Ȯ�����ּ���." };
		const char PRIMARY_INVALID[]{ "������ ��Ģ�� ���� �ʽ��ϴ�." };
	}
}