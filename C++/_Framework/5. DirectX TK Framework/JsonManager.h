#pragma once

// JsonManager: json 파일을 로딩 및 관리합니다. 파싱된 json 파일을 반환하며 중복 로딩을 방지합니다.

namespace rapidjson
{
	using WDocument = GenericDocument<rapidjson::UTF16<>>;
	using WValue = GenericValue<UTF16<>>;
}

namespace DX
{
	class JsonManager
	{
	public:
		static JsonManager& Instance()
		{
			static JsonManager instance;
			return instance;
		}

		JsonManager();
		~JsonManager();

		JsonManager(JsonManager&&) = delete;
		JsonManager& operator =(JsonManager&&) = delete;

		JsonManager(JsonManager const&) = delete;
		JsonManager& operator =(JsonManager const&) = delete;

	public:
		rapidjson::WDocument* CreateDocument(LPCWSTR filename);
		rapidjson::WDocument* GetDocument(LPCWSTR filename);
		void Remove(LPCWSTR filename);
		void ReleaseAll();

	private:
		std::map<std::wstring, std::unique_ptr<rapidjson::WDocument>> m_documents;
	};
}

