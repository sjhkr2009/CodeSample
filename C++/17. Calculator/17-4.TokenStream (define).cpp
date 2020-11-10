#include "Calculator.h"
#include <iostream>

Token TokenStream::Get()
{
	if (mFull)
	{
		mFull = false;
		return mBuffer;
	}

	char kind;
	Token token;

	std::cin >> kind;

	switch (kind)
	{
		case 'q':case 'Q':
		case '=':case ';':
		case '+':case '-':case '*':case '/':case '(':case ')':
			token.type = kind;
			break;

		case '1':case '2':case '3':case '4':case '5':
		case '6':case '7':case '8':case '9':case '0':case '.':
			token.type = 'N';
			std::cin.putback(kind);
			std::cin >> token.value;
			break;
		
		default:
			std::cerr << "예상하지 못한 토큰 " << kind << " 을 감지했습니다." << std::endl;
			throw std::logic_error("잘못된 토큰입니다.");
	}
	return token;
}

void TokenStream::PutBack(Token t)
{
	if (mFull)
	{
		throw std::logic_error("버퍼에 이미 토큰이 있습니다.");
	}
	mBuffer = t;
	mFull = true;
}


double Calculator::Expression()
{
	double value{ Term() };
	Token token{ mTokenStream.Get() };

	while (true)
	{
		switch (token.type)
		{
			case '+':
				value += Term();
				break;
			case '-':
				value -= Term();
				break;
			default:
				mTokenStream.PutBack(token);
				return value;
		}
		token = mTokenStream.Get();
	}
}

double Calculator::Term()
{
	double value{ Primary() };
	Token token{ mTokenStream.Get() };

	while (true)
	{
		switch (token.type)
		{
			case '*':
				value *= Primary();
				break;
			case '/':
			{
				double d = Primary();
				if (d == 0)
				{
					throw std::logic_error("Error: Divide by Zero.");
				}
				value /= d;
				break;
			}
			default:
				mTokenStream.PutBack(token);
				return value;
		}
		token = mTokenStream.Get();
	}
}

double Calculator::Primary()
{
	
	Token token{ mTokenStream.Get() };

	switch (token.type)
	{
		case 'N':
			return token.value;
		case '(':
		{
			double d = Expression();
			token = mTokenStream.Get();
			if (token.type != ')')
			{
				throw std::logic_error("괄호 안닫음!");
			}
			return d;
		}
		default:
			std::cerr << "규칙 위반 기호 : " << token.type << std::endl;
			throw std::logic_error("수식이 규칙에 맞지 않습니다.");
	}
}

bool Calculator::IsQuit(char cmd) { return (cmd == QUIT || cmd == QUIT_SUB); }

bool Calculator::IsCalcEnd(char cmd) { return (cmd == CALC_END || cmd == CALC_END_SUB); }

Calculator::Calculator() :
	mTokenStream { TokenStream() } {}

void Calculator::Start()
{
	double value{ 0 };
	//Token token;

	while (std::cin)
	{
		Token token = mTokenStream.Get();
		if (IsQuit(token.type))
		{
			break;
		}
		else if (IsCalcEnd(token.type))
		{
			std::cout << "= " << value << std::endl;
			continue;
		}
		else
		{
			mTokenStream.PutBack(token);
		}
		value = Expression();
	}
}