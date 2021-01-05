#include "PCH.h"

#include <fstream>
#include <sstream>

using namespace DX;
using namespace rapidjson;

JsonManager::JsonManager()
{
}

JsonManager::~JsonManager()
{
	m_documents.clear();
}

/// <summary>
/// json ������ �ҷ��� �Ľ��ϰ� �����մϴ�. ������ ������ ��ȯ�մϴ�.
/// �̹� ����� ������ �ٽ� �ҷ����� �ʰ� ����� ���� ������ ��ȯ�մϴ�.
/// </summary>
/// <param name="filename">: �ҷ��� ���� ��� �� ���ϸ��� �Է��մϴ�.</param>
/// <returns></returns>
WDocument* JsonManager::CreateDocument(LPCWSTR filename)
{
	auto result = m_documents.insert(std::make_pair(filename, nullptr));

	if (result.second == true)
	{
		std::wstringstream stream;
		std::wifstream file(filename, std::wifstream::binary);

		if (!file)
		{
			throw std::exception("Fail to Load Json File!");
			file.close();
			return nullptr;
		}

		stream << file.rdbuf();
		file.close();

		result.first->second = std::move(std::make_unique<WDocument>());
		result.first->second.get()->Parse(stream.str().c_str());
	}

	return result.first->second.get();
}

/// <summary>
/// json ���� ������ ��ȯ�մϴ�. ������ ���� ��� �ش� ����� ������ �Ľ��� �� �����ϰ� ��ȯ�մϴ�.
/// (CreateDocument�� �����ϰ� �����մϴ�)
/// </summary>
/// <param name="filename">: �ҷ��� ���� ��� �� ���ϸ��� �Է��մϴ�.</param>
/// <returns></returns>
rapidjson::WDocument* DX::JsonManager::GetDocument(LPCWSTR filename)
{
	return CreateDocument(filename);
}

void JsonManager::Remove(LPCWSTR filename)
{
	m_documents.erase(filename);
}

void JsonManager::ReleaseAll()
{
	m_documents.clear();
}
