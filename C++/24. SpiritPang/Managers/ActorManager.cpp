#include "PCH.h"

using namespace DirectX;
using namespace DX;

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
	ReleaseAll();
}

void ActorManager::OnDeviceLost()
{
	for (auto& layer : m_actors)
	{
		for (auto& actor : layer.second)
		{
			actor->OnDeviceLost();
		}
	}
}

void ActorManager::OnDeviceRestored()
{
	for (auto& layer : m_actors)
	{
		for (auto& actor : layer.second)
		{
			actor->OnDeviceRestored();
		}
	}
}

void ActorManager::Initialize()
{
}

void ActorManager::ReleaseAll()
{
	for (auto& layer : m_actors)
	{
		for (auto& actor : layer.second)
		{
			actor.reset();
		}
		layer.second.clear();
	}
	m_actors.clear();
}

void ActorManager::Update(double delta)
{
	for (auto& layer : m_actors)
	{
		for (auto& actor : layer.second)
		{
			actor->Update(delta);
		}
	}
}

void ActorManager::Render(DirectX::SpriteBatch* pSpriteBatch)
{
	assert(pSpriteBatch != nullptr);
	for (auto& layer : m_actors)
	{
		for (auto& actor : layer.second)
		{
			actor->Render(pSpriteBatch);
		}
	}
}

void ActorManager::Delete(int layer, Actor* pActor)
{
	auto itr = m_actors[layer].begin();
	auto endItr = m_actors[layer].end();

	while (itr != endItr)
	{
		if (itr->get() == pActor)
		{
			itr->reset();
			m_actors[layer].erase(itr);
			break;
		}
		++itr;
	}

	for (auto& actor : m_actors[layer])
	{
		if (pActor == actor.get())
		{
			m_actors[layer].remove(actor);
			actor.reset();
			return;
		}
	}
	Utility::DebugString(L"[ActorManager::Delete] Can't find delete target!");
}
