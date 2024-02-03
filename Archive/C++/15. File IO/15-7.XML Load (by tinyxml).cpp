#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2.h"

// XML 데이터 불러오기 - tinyxml 사용

using namespace tinyxml2;

class Sprite
{
public:
	std::string n;
	int x;
	int y;
	int w;
	int h;
};

void LoadToXML(const char* filename, std::vector<Sprite>& sprites)
{
	// XMLDocument : 문서를 들고 있을 클래스
	XMLDocument doc;
	
	// 파일을 읽어오게 한다. LoadFile 함수는 성공 시 XML_SUCCESS를, 아닐 경우 에러 코드를 반환한다.
	if (doc.LoadFile(filename) != XML_SUCCESS)
	{
		std::cerr << "파일을 찾을 수 없습니다!" << std::endl;
		return;
	}

	// FirstChildElement(a) 함수는 문서의 첫 번째 태그 a를 찾아온다. 매개변수가 없다면 첫 번째 자식 태그를 찾아온다.
	auto root = doc.FirstChildElement("TextureAtlas");

	// root의 자식 태그는 sprite 뿐이므로, 첫 번째 매개변수 없이 태그를 찾으면 된다. sprite가 null일 때까지 반복한다.
	// NextSiblingElement()은 다음 형제 태그(같은 부모를 가진 태그)를 찾아온다.
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		Sprite sprite;

		// Attribute(a) 함수는 해당 태그의 속성 중에서 a 이름을 가진 속성을 반환한다.
		// 기본적으로 문자열을 반환하지만, Int-, Float- 등을 붙여 원하는 형태로 형변환된 속성을 반환받을 수 있다.
		sprite.n = element->Attribute("n");
		sprite.w = element->IntAttribute("w");
		sprite.h = element->IntAttribute("h");
		sprite.x = element->IntAttribute("x");
		sprite.y = element->IntAttribute("y");
		
		sprites.push_back(sprite);
	}
}

int main()
{
	std::vector<Sprite> mySprites;

	LoadToXML("Data/mydata.xml", mySprites);

	for (auto e : mySprites)
	{
		std::cout << e.n << " : " << e.x << "," << e.y << std::endl;
	}
}