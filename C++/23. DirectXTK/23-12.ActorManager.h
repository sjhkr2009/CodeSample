#pragma once

namespace DX
{
	class ActorManager : public IDeviceNotify
	{
	public:
		static ActorManager& Instance()
		{
			static ActorManager instance;
			return instance;
		}

		ActorManager();
		~ActorManager();

		ActorManager(ActorManager&&) = delete;
		ActorManager& operator =(ActorManager&&) = delete;

		ActorManager(ActorManager const&) = delete;
		ActorManager& operator =(ActorManager const&) = delete;

	public:
		void OnDeviceLost() override;
		void OnDeviceRestored() override;

		void Initialize();
		void ReleaseAll();

		void Update(double delta);
		void Render(DirectX::SpriteBatch* pSpriteBatch);

		template<class T>
		T* Create();
		template<class T>
		T* Create(const int layer, const LPCWSTR textureName, const LPCWSTR sheetName = nullptr, const LPCWSTR animationName = nullptr);
		template<class T, class... Types>
		T* Create(int layer, Types&& ... args);

		void Delete(int layer, Actor* pActor);

	private:
		std::map<int, std::list<std::unique_ptr<Actor>>> m_actors;
	};
	template<class T>
	inline T* ActorManager::Create()
	{
		m_actors[0].push_back(std::make_unique<T>());
		return dynamic_cast<T*>(m_actors[0].back().get());
	}
	template<class T>
	inline T* ActorManager::Create(const int layer, const LPCWSTR textureName, const LPCWSTR sheetName, const LPCWSTR animationName)
	{
		m_actors[layer].push_back(std::make_unique<T>());
		T* pActor = dynamic_cast<T*>(m_actors[layer].back().get());

		pActor->LoadSpriteSheet(textureName, sheetName);

		if (animationName != nullptr)
		{
			AnimatedActor* pAnim = dynamic_cast<AnimatedActor*>(pActor);
			if (pAnim)
			{
				pAnim->LoadAnimation(animationName);
			}
		}
		
		return pActor;
	}
	template<class T, class ...Types>
	inline T* ActorManager::Create(int layer, Types&& ...args)
	{
		m_actors[layer].push_back(std::unique_ptr<T>(new T(std::forward<Types>(args)...)));
		return dynamic_cast<T*>(m_actors[layer].back().get());
	}
}
