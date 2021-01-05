#pragma once

// ActorManager: ���� �� ��� Actor���� �����մϴ�. Actor�� ���� �� ����, Update/Render �Լ��� �ϰ� ȣ���� ����մϴ�.

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

	template<class T, class ...Types>
	inline T* ActorManager::Create(int layer, Types&& ...args)
	{
		m_actors[layer].push_back(std::unique_ptr<T>(new T(std::forward<Types>(args)...)));
		return dynamic_cast<T*>(m_actors[layer].back().get());
	}
}
