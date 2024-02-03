#include "Calculator.h"
#include "Global.h"

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
			std::cerr << Message::Parenthasis::TOKEN_ERROR << " : " << kind << std::endl;
			ThrowInputErr(Message::Error::TOKEN_INVALID);
	}
	return token;
}

void TokenStream::PutBack(Token t)
{
	if (mFull)
	{
		ThrowInputErr(Message::Error::TOKEN_OVERLAP);
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
					ThrowInputErr(Message::Error::DEVIDE_BY_ZERO);
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
				ThrowInputErr(Message::Error::PARENTHASIS_OPENED);
			}
			return d;
		}
		case '-':
			return -Primary();
		case '+':
			return Primary();
		default:
			std::cerr << Message::Parenthasis::PRIMARY_ERROR << " : " << token.type << std::endl;
			ThrowInputErr(Message::Error::PRIMARY_INVALID);
	}
}

bool Calculator::IsQuit(char cmd) { return (cmd == Commands::QUIT || cmd == Commands::QUIT_SUB); }

bool Calculator::IsCalcEnd(char cmd) { return (cmd == Commands::CALC_END || cmd == Commands::CALC_END_SUB); }

Calculator::Calculator() :
	mTokenStream { TokenStream() } {}

void Calculator::Start()
{
	double value{ 0 };
	Token token;

	std::cout << Message::Common::MENUAL << std::endl;
	std::cout << Constants::PROMPT;
	while (std::cin)
	{
		token = mTokenStream.Get();
		if (IsQuit(token.type))
		{
			break;
		}
		else if (IsCalcEnd(token.type))
		{
			if (token.type != '=')
			{
				std::cout << Constants::RESULT;
			}
			std::cout << value << std::endl;
			std::cout << Constants::PROMPT;
			continue;
		}
		else
		{
			mTokenStream.PutBack(token);
		}
		value = Expression();
	}
}