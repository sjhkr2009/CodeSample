#include <iostream>
#include <vector>

// 계산기 만들기

// 문법 규칙 적용

// C++에서 산술 연산을 위한 구문 분석에는 아래와 같은 문법이 적용된다.
/*
	Expression(표현식):
		용어
		표현식 + 용어
		표현식 - 용어
	Term(용어):
		기본식
		용어 * 기본식
		용어 / 기본식
		용어 % 기본식
	Primary(기본식):
		숫자
		"("와 ")"로 둘러싸인 표현식
	Number(숫자):
		부동소수점 리터럴
*/

// 곱셈이 덧셈에 우선하는 등 우선순위를 반영하려면, 
// 이전처럼 입력 순서대로 계산하는 대신 각 항을 토큰화하여 컨테이너에 넣어 두고 연산을 시작해야 한다.
class Token
{
public:
	char kind;		// 토큰의 종류 (숫자일 경우 N을 넣기로 한다)
	double value;	// 토큰의 값 (연산자일 경우 값을 할당하지 않음)

public:
	// 함수에서 처리할 수 없는 기호를 보면 다시 스트림에 되돌려놓기 위한 함수
	void PutBack()
	{
		if (kind != 'N')
		{
			std::cin.putback(kind);
		}
	}
};

Token GetToken();
double Expression();
double Term();
double Primary();

int main()
{
	try
	{
		while (std::cin)
		{
			std::cout << "계산식을 입력하세요. (예: 2 + 3 =)" << std::endl << "> ";
			// = 기호는 Expression 및 하위 함수에서 처리할 수 없으므로 입력 스트림에서 빼 준다.
			if (std::cin.peek() == '=')
			{
				char a{};
				std::cin >> a;
			}
			std::cout << "= " << Expression() << std::endl;
		}
	}
	catch (std::exception e)
	{
		std::cerr << "에러가 발생했습니다 : " << e.what() << std::endl;
		return 1;
	}
}

Token GetToken()
{
	//char 변수 하나를 선언하고 값을 받아온다.
	char kind;
	std::cin >> kind;

	Token t;
	switch (kind)
	{
		// 연산자일 경우 토큰의 타입을 해당 연산자로 지정하고 반환한다.
		case '=':
		case '+':case '-':case '*':case '/':case '(':case ')':
			t.kind = kind;
			break;

		// 숫자일 경우 받아온 숫자를 입력 스트림에 재삽입(cin.putback)하고, 토큰의 value 타입에 다시 값을 받아온 후 반환한다.
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9': case '.':
			t.kind = 'N';
			std::cin.putback(kind);
			std::cin >> t.value;
			break;

		// 둘 다 아닐 경우 에러 메시지를 출력한다.
		default:
			std::cerr << "예상되지 않은 값이 입력되었습니다 : " << kind << std::endl;
			throw std::logic_error("알 수 없는 값을 감지했습니다.");
	}
	
	return t;
}

// 표현식 구현
double Expression()
{
	double lValue{ Term() };
	Token token{ GetToken() };

	while (true)
	{
		if (token.kind == '+')
		{
			lValue += Term();
		}
		else if (token.kind == '-')
		{
			lValue -= Term();
		}
		else
		{
			token.PutBack();
			return lValue;
		}

		token = GetToken();
	}
}

// 용어 구현
double Term()
{
	double lValue{ Primary() };
	Token token{ GetToken() };

	while (true)
	{
		if (token.kind == '*')
		{
			lValue *= Primary();
		}
		else if (token.kind == '/')
		{
			double d{ Primary() };
			if (d == 0)
			{
				throw std::logic_error("Devide by zero!");
			}
			lValue /= d;
		}
		else
		{
			token.PutBack();
			return lValue;
		}

		token = GetToken();
	}
}

// 기본식 구현
double Primary()
{
	Token token{ GetToken() };

	if (token.kind == 'N')
	{
		return token.value;
	}
	else if (token.kind == '(')
	{
		double value{ Expression() };
		if (GetToken().kind != ')')
		{
			throw std::logic_error("괄호 안닫음?");
		}
		return value;
	}
	else
	{
		throw std::logic_error("Primary : 숫자와 괄호 외의 처리할 수 없는 토큰을 감지했습니다.");
	}
}
