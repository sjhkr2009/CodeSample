#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// 파일 읽어와서 구조체에 저장
// 한 줄씩 읽는 기능(std::getline)을 위해 include sstream 필요

class Monster
{
private:
	std::string mName;
	int mLevel;
	int mHp;
	int mMp;

public:
	Monster(std::string name, int level, int hp, int mp) :
		mName{name},
		mLevel{level},
		mHp{hp},
		mMp{mp}
	{

	}
};

bool LoadFile(const char* filename)
{
	std::ifstream ist;

	ist.exceptions(std::ifstream::badbit);
	
	try
	{
		ist.open(filename);
		std::string buffer;

		// 우선 첫 줄인 헤더 파일을 읽어온다. (여기서 헤더로 할 동작은 없으니 바로 다음 줄을 읽을 것이다)
		// getline으로 한 줄을 읽어와서 buffer에 저장한다.
		std::getline(ist, buffer);

		// 파일 맨 끝이 아닐 경우 반복한다.
		while (!ist.eof())
		{
			std::getline(ist, buffer);

			// 읽어온 한 줄의 내용을 buffer에 저장한다.
			std::stringstream ss(buffer);
			
			// 해당 줄의 요소를 하나씩 저장할 리스트를 생성한다.
			std::vector<std::string> args;

			// 여기서는 한 객체(몬스터)당 저장된 정보가 4개(이름,레벨,HP,MP)이므로 4회 반복한다. 정보의 내용과 개수 등은 보통 헤더에 들어있다.
			for (int i = 0; i < 4; i++)
			{
				std::string token;

				// 해당 스트링스트림을 ',' 앞까지만 읽어 저장한다. (C#의 .split() 기능과 유사)
				// 읽어오면서 동시에 처리해도 되겠지만, 해당 줄을 읽고 나서(std::getline(ist, buffer) 부분 바로 뒤) 예외 처리를 하기 때문에 나누어 작업한다.
				//		ㄴ 예를 들어, 데이터 형식이 맞는지, 콤마가 3개 있는지 등...
				std::getline(ss, token, ',');

				args.push_back(token);
			}

			// string을 다른 형태로 바꿀 때는 std::sto- 계열의 함수를 사용한다. int로 변경하려면 stoi()
			Monster monster(args[0], std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]));
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