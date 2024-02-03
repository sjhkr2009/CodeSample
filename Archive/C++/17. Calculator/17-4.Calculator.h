#pragma once
class Token
{
public:
	char type;
	int value;
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
	
	const char QUIT{ 'q' };
	const char QUIT_SUB{ 'Q' };
	const char CALC_END{ ';' };
	const char CALC_END_SUB{ '=' };

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