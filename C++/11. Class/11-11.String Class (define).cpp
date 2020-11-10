#include "String.h"

String::String() : mLength{ 0 }
{
	mString = new char[1]{ '\0' };
}

String::String(const char* content)
{
	int index{};
	while (content[index] != '\0')
	{
		++index;
	}

	mString = new char[index + 1];
	mLength = index;

	mString[mLength] = '\0';

	for (index = 0; index < mLength; ++index)
	{
		mString[index] = content[index];
	}
}

String::String(int length) : mLength{ length }
{
	mString = new char[length + 1];
	for (int i = 0; i < mLength; i++)
	{
		mString[i] = '\0';
	}
}

String::String(const String& other)
{
	mLength = other.GetLength();
	mString = new char[mLength + 1];

	mString[mLength] = '\0';

	for (int i = 0; i < mLength; i++)
	{
		mString[i] = other[i];
	}
}

String::~String()
{
	delete mString;
	mString = nullptr;
	mLength = 0;
}

char String::operator[](int index) const
{
	return (index > mLength) ? mString[mLength] : mString[index];
}

char& String::operator[](int index)
{
	return (index > mLength) ? mString[mLength] : mString[index];
}

String String::operator+(const String& string)
{
	int newLength{ mLength + string.GetLength() };
	String result(newLength);

	for (int i = 0; i < mLength; i++)
	{
		result[i] = mString[i];
	}
	for (int i = 0; i < string.GetLength(); i++)
	{
		result[mLength + i] = string[i];
	}

	result[newLength] = '\0';

	return result;
}

String String::operator=(const String& string)
{
	if (&string == this)
	{
		return *this;
	}

	delete[] mString;
	mLength = string.GetLength();
	mString = new char[mLength + 1];

	for (int i = 0; i < mLength; i++)
	{
		mString[i] = string[i];
	}
	mString[mLength] = '\0';

	return *this;
}

void String::operator += (const String& string)
{
	*this = *this + string;
}

std::ostream& operator<<(std::ostream& os, const String string)
{
	os << string.mString;
	return os;
}
