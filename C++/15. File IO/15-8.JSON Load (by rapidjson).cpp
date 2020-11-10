#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <rapidjson/document.h>

using namespace rapidjson;

// JSON 데이터 불러오기 - rapidjson 사용
// 상단 바의 [프로젝트] - [NuGet 패키지 관리]의 '찾아보기'에서 원하는 라이브러리를 찾아서 이 프로젝트에 추가할 수 있다.

class Sprite
{
public:
	std::string n;
	int x;
	int y;
	int w;
	int h;
};

std::stringstream ReadJSON(const char* filename)
{
	// JSON은 파일의 문자열을 -> 파싱(Parsing)해서 -> document로 변환하는 과정을 거친다.
	// 즉 파일을 열어서 모든 요소를 문자열로 긁어와야 한다.

	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// std::string은 한 줄만 읽어오므로, 스트링 스트림으로 읽어온다. (include sstream 필요)
	std::stringstream ss;

	try
	{
		ifs.open(filename);
		ss << ifs.rdbuf(); // rdbuf(=read buffer)로 파일의 내용을 통째로 읽어온다.
		ifs.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "파일을 읽는 도중 에러가 발생했습니다." << std::endl;
		ifs.close();
		return std::stringstream{};
	}
	return ss;
}

void LoadJSON(const char* filename, std::vector<Sprite>& sprites)
{
	std::stringstream ss{ ReadJSON(filename) };
	if (ss.str() == std::stringstream{}.str())
	{
		return;
	}

	Document doc;
	doc.Parse(ss.str().c_str()); //c 스타일만 지원하므로, 스트림을 std::string으로 변환 후, const char*로 다시 변환한다.

	auto array = doc["sprites"].GetArray();
	for (auto& e : array)
	{
		Sprite sprite;

		sprite.n = e["name"].GetString();
		sprite.h = e["h"].GetInt();
		sprite.w = e["w"].GetInt();
		sprite.x = e["x"].GetInt();
		sprite.y = e["y"].GetInt();

		sprites.push_back(sprite);
	}

}

int main()
{
	std::vector<Sprite> mySprites;

	LoadJSON("Data/mydata.json", mySprites);

	for (auto e : mySprites) 
	{
		std::cout << e.n << " : " << e.x << ' ' << e.y << std::endl;
	}
}
