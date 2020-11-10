#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// 바이너리 데이터로 저장/로드하기

struct Header
{
	int version{ 1 };
	int itemCount{};
};

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
	{}
	Monster() : mName{}, mLevel{}, mHp{}, mMp{}
	{}

	void Print() { std::cout << mName << "," << mLevel << "," << mHp << "," << mMp << std::endl; }

	// 바이트(Binary Data)로 저장 및 로드
	std::ifstream& ReadBinary(std::ifstream& ifstream);
	std::ofstream& WriteBinary(std::ofstream& ofstream);
};

// 함수 내에서 private멤버에 접근하지 않아도 되므로, 지난 시간에 friend를 이용해 오버로딩한 연산자는 전역으로 선언 및 정의한다.
std::ifstream& operator>>(std::ifstream& inputStream, Monster& monster)
{
	return monster.ReadBinary(inputStream);
}
std::ofstream& operator<<(std::ofstream& outputStream, Monster& monster)
{
	return monster.WriteBinary(outputStream);
}

/// <summary>
/// 저장된 데이터 읽어오기. 스트림에 순서대로 저장되어 있으므로, 저장할 때와 읽어올 때의 순서와 크기가 동일해야 한다.
/// </summary>
std::ifstream& Monster::ReadBinary(std::ifstream& ifstream)
{
	// ifstream.read(읽어올 주소, 크기) 를 사용하여 데이터를 읽어올 수 있다.

	int strLen{};
	ifstream.read(reinterpret_cast<char*>(&strLen), sizeof(int));
	mName.resize(strLen);
	ifstream.read(const_cast<char*>(mName.c_str()), strLen);
	ifstream.read(&mName[0], strLen);

	ifstream.read(reinterpret_cast<char*>(&mLevel), sizeof(int));
	ifstream.read(reinterpret_cast<char*>(&mHp), sizeof(int));
	ifstream.read(reinterpret_cast<char*>(&mMp), sizeof(int));

	return ifstream;
}

/// <summary>
/// 읽어온 데이터를 바이트로 저장
/// </summary>
std::ofstream& Monster::WriteBinary(std::ofstream& ofstream)
{
	// 문자열은 문자열의 길이와 내용을 저장해야 한다.
	// ofstream의 매개변수는 char 포인터이므로, 문자열의 길이를 구한 후 해당 주소를 char*로 변환하여 저장한다. 변환에는 reinterpret_cast가 사용된다.
	int strLen{ static_cast<int>(mName.size()) };
	ofstream.write(reinterpret_cast<char*>(&strLen), sizeof(int));

	// 문자는 .c_str()로 변환 후 문자열 길이에 맞게 저장한다.
	ofstream.write(mName.c_str(), strLen);

	// 나머지 int형 자료들은 주소를 char*로 변환하여 저장한다. 변환에는 reinterpret_cast가 사용된다.
	ofstream.write(reinterpret_cast<char*>(&mLevel), sizeof(int));
	ofstream.write(reinterpret_cast<char*>(&mHp), sizeof(int));
	ofstream.write(reinterpret_cast<char*>(&mMp), sizeof(int));

	return ofstream;
}

bool SaveFile(const char* filename, std::vector<Monster>& monsters)
{
	std::ofstream ostream;
	ostream.exceptions(std::ofstream::badbit | std::ofstream::failbit);

	try
	{
		// 1. 파일 열기
		ostream.open(filename, std::ostream::binary);

		// 2. Header 저장
		Header header{ 1, static_cast<int>(monsters.size()) };
		ostream.write(reinterpret_cast<char*>(&header), sizeof(Header));

		// 3. Monster 목록 저장
		for (auto& item : monsters)
		{
			ostream << item;
		}

		// 4. 파일 닫기
		ostream.close();
	}
	catch (std::ofstream::failure e)
	{
		std::cerr << std::endl << "파일을 저장할 수 없습니다 : " << e.what() << std::endl;
		ostream.close();
		return false;
	}
	return true;
}
bool LoadFile(const char* filename, std::vector<Monster>& monsters)
{
	std::ifstream istream;
	istream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		// 1. 파일 열기
		istream.open(filename, std::ostream::binary);

		// 2. Header 읽기
		Header header;
		istream.read(reinterpret_cast<char*>(&header), sizeof(Header));

		// 3. Monster 목록 읽기
		for (int i = 0; i < header.itemCount; i++)
		{
			Monster monster;
			istream >> monster;
			monsters.push_back(monster);
		}

		// 4. 파일 닫기
		istream.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << std::endl << "파일을 읽을 수 없습니다 : " << e.what() << std::endl;
		istream.close();
		return false;
	}
	
	return true;
}

int main()
{
	std::vector<Monster> monsters
	{
		{"SLIME",1,1,1},
		{"WOLF",5,5,5 },
		{"DEMON",10,10,10}
	};

	// 참고로 프로젝트 폴더 안에 Data 폴더가 없으면 에러가 나니 주의
	SaveFile("Data/SimpleBinaryData.dat", monsters);

	monsters.clear();
	LoadFile("Data/SimpleBinaryData.dat", monsters);
	
	for (auto e : monsters)
	{
		e.Print();
	}
}

