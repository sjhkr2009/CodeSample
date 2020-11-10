#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

// XML 데이터 불러오기 - 정규표현식 사용

// 데이터에서 큰따옴표로 시작하고 끝나는 요소를 찾아오는 정규표현식( "([^"]*)", 따옴표는 예외처리)을 사용한다.
// 정규표현식에 대해서는 13-15 코드 참고

// 다만 XML에서 속성들의 순서는 바뀔 수 있으므로, 순서에 의존하는 이런 방식의 읽기는 실제로 사용하기엔 한계가 있다.

class Sprite
{
public:
	std::string n;
	int x;
	int y;
	int w;
	int h;
};

void LoadXML(const char* filename, std::vector<Sprite>& sprites)
{
	sprites.clear();

	// 1. 파일 읽기
	std::ifstream file(filename);

	// 2. 패턴 생성
	std::regex pattern("\"([^\"]*)\"");


	// 3. 파일 한줄씩 읽기
	std::string line{};

	while (!file.eof())
	{
		std::getline(file, line);

		// 한 줄을 읽고, find로 해당 줄에서 <sprite 로 시작하는 태그 부분만 찾아온다. 찾지 못하면 std::string::npos를 반환한다.
		auto result = line.find("<sprite");
		if (result == std::string::npos)
		{
			continue;
		}

		//	3-1. 패턴으로 목록을 찾아서 반복자를 생성
		std::sregex_iterator current(line.begin(), line.end(), pattern);
		std::sregex_iterator end; // 이터레이터는 기본 생성자로 자동으로 end를 가리킨다.

		Sprite sprite;
		int index{};

		//	3-2. 반복자를 사용해 순회
		while (current != end)
		{
			//std::cout << current->str() << std::endl; // ->str()을 통해 std::string으로 변환할 수 있다.
			std::cout << (*current)[0] << ' ' << (*current)[1] << std::endl; // (*현재 이터레이터) 에서 [0]은 읽은 값 그대로를, [1]은 따옴표를 빼고 가지고 있다.
			
			//	3-3. Sprite 개체 대입
			std::string token{ (*current)[1] };

			switch (index)
			{
				case 0:
					sprite.n = token;
					break;
				case 1:
					sprite.x = std::stoi(token);
					break;
				case 2:
					sprite.y = std::stoi(token);
					break;
				case 3:
					sprite.w = std::stoi(token);
					break;
				case 4:
					sprite.h = std::stoi(token);
					break;
				default:
					break;
			}
			index++;
			current++;
		}

		sprites.push_back(sprite);
	}

	file.close();
}

int main()
{
	std::vector<Sprite> mySprites;

	LoadXML("Data/mydata.xml", mySprites);

	for (auto e : mySprites)
	{
		std::cout << e.n << " : " << e.x << "," << e.y << std::endl;
	}
}
