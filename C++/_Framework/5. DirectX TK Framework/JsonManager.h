#pragma once

// JsonManager: json ������ �ε� �� �����մϴ�. �Ľ̵� json ������ ��ȯ�ϸ� �ߺ� �ε��� �����մϴ�.

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

