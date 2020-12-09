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
/// json 파일을 불러와 파싱하고 저장합니다. 저장한 정보를 반환합니다.
/// 이미 저장된 파일은 다시 불러오지 않고 저장된 파일 정보를 반환합니다.
/// </summary>
/// <param name="filename">: 불러올 파일 경로 및 파일명을 입력합니다.</param>
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
/// json 파일 정보를 반환합니다. 정보가 없을 경우 해당 경로의 파일을 파싱한 후 저장하고 반환합니다.
/// (CreateDocument과 동일하게 동작합니다)
/// </summary>
/// <param name="filename">: 불러올 파일 경로 및 파일명을 입력합니다.</param>
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
