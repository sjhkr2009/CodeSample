#pragma once

/*
	[간단한 계산기]

	<문법>
	Statement:
		Expression
		Print
		Quit
	End Od Line:
		; =
	Quit:
		q Q
	Expression:
		Term
		Expression + Term
		Expression - Term
	Term:
		Primary
		Term * Primary
		Term / Primary
	Primary:
		Number
		( Expression )
		-Primary
		+Primary
	Number:
		double floating point literal
*/


class Token
{
public:
	char type;
	double value;
};

class TokenStream
{
private:
	bool mFull{ false };
	Token mBuffer;

public:
	Token Get();
	void PutBack(Token t);
};

class Calculator
{
private:
	TokenStream mTokenStream;

private:
	double Expression();
	double Term();
	double Primary();
	bool IsQuit(char cmd);
	bool IsCalcEnd(char cmd);

public:
	Calculator();

	void Start();
};