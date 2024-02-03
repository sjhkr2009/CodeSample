#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// 파일 읽어와서 구조체에 저장 - 연산자 오버로딩으로 만들기

class Monster
{
private:
	std::string mName;
	int mLevel;
	int mHp;
	int mMp;

public:
	Monster(std::string name, int level, int hp, int mp) :
		mName{ name },
		mLevel{ level },
		mHp{ hp },
		mMp{ mp }
	{
	}
	Monster() {}

	// 입력 연산자를 만들어줄 것이므로 istream을 사용한다.
	friend std::istream& operator >> (std::istream& inputStream, Monster& monster);
	friend std::ostream& operator << (std::ostream& outputStream, Monster& monster);
};

std::istream& operator>>(std::istream& inputStream, Monster& monster)
{
	std::string buffer;

	try 
	{
		std::getline(inputStream, buffer, ',');
		monster.mName = buffer;
		std::getline(inputStream, buffer, ',');
		monster.mLevel = std::stoi(buffer);
		std::getline(inputStream, buffer, ',');
		monster.mHp = std::stoi(buffer);

		// 마지막엔 쉼표가 없으니 남은 줄을 그대로 읽으면 된다.
		std::getline(inputStream, buffer);
		monster.mMp = std::stoi(buffer);
	}
	catch (std::exception e) // 모든 예외를 잡으려면 std::exception을 사용한다.
	{
		std::cerr << "파일 형식이 잘못되었습니다!" << std::endl;
	}
	return inputStream;
}

std::ostream& operator<<(std::ostream& outputStream, Monster& monster)
{
	try
	{
		outputStream << monster.mName << "," <<
			monster.mLevel << "," <<
			monster.mLevel << "," <<
			monster.mLevel << std::endl;
	}
	catch (std::exception e)
	{
		std::cerr << "파일에 저장할 수 없습니다." << std::endl;
	}
	return outputStream;
}

bool LoadFile(const char* filename)
{
	std::ifstream ist;
	ist.exceptions(std::ifstream::badbit);

	Monster monster{};

	try
	{
		ist.open(filename);
		std::string buffer;

		int index{};

		while (!ist.eof())
		{
			// peek()로 다음 내용을 확인해본 후, 마지막 부분이면 반복문을 중단한다.
			if (ist.peek() == EOF)
			{
				break;
			}
			
			ist >> monster;
			std::cout << monster;
		}

		ist.close();
	}
	catch (std::ifstream::failure e)
	{
		if (ist.eof())
		{
			std::cerr << std::endl << "파일 읽기를 완료했습니다." << std::endl;
		}
		else
		{
			std::cerr << std::endl << "파일을 읽는 도중 예외가 발생했습니다 : " <<
				e.what() << std::endl;
		}

		ist.close();
		return false;
	}

	return true;
}

int main()
{
	LoadFile("SimpleData.txt");
}

