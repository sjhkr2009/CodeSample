#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2.h"

// XML ������ �ҷ����� - tinyxml ���

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
	// XMLDocument : ������ ��� ���� Ŭ����
	XMLDocument doc;
	
	// ������ �о���� �Ѵ�. LoadFile �Լ��� ���� �� XML_SUCCESS��, �ƴ� ��� ���� �ڵ带 ��ȯ�Ѵ�.
	if (doc.LoadFile(filename) != XML_SUCCESS)
	{
		std::cerr << "������ ã�� �� �����ϴ�!" << std::endl;
		return;
	}

	// FirstChildElement(a) �Լ��� ������ ù ��° �±� a�� ã�ƿ´�. �Ű������� ���ٸ� ù ��° �ڽ� �±׸� ã�ƿ´�.
	auto root = doc.FirstChildElement("TextureAtlas");

	// root�� �ڽ� �±״� sprite ���̹Ƿ�, ù ��° �Ű����� ���� �±׸� ã���� �ȴ�. sprite�� null�� ������ �ݺ��Ѵ�.
	// NextSiblingElement()�� ���� ���� �±�(���� �θ� ���� �±�)�� ã�ƿ´�.
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		Sprite sprite;

		// Attribute(a) �Լ��� �ش� �±��� �Ӽ� �߿��� a �̸��� ���� �Ӽ��� ��ȯ�Ѵ�.
		// �⺻������ ���ڿ��� ��ȯ������, Int-, Float- ���� �ٿ� ���ϴ� ���·� ����ȯ�� �Ӽ��� ��ȯ���� �� �ִ�.
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