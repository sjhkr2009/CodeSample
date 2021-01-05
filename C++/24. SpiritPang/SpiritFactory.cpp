#include "PCH.h"
#include "SpiritFactory.h"

using namespace DX;
using namespace SpiritPang;

Spirit* SpiritFactory::CreateSpirit(int type, float size)
{
	Spirit* pSpirit = ActorManager::Instance().Create<Spirit>(
		static_cast<int>(GameBase::Layer::Character),
		static_cast<Spirit::Type>(type)
		);
	pSpirit->SetPivot(0.f, 0.f);
	pSpirit->SetSize(size);

	ParticlePang* pParticle = CreateDestroyParticle(size);
	pSpirit->SetParticle(pParticle);
	
	return pSpirit;
}

ParticlePang* SpiritFactory::CreateDestroyParticle(float originSize)
{
	ParticlePang* pParticle = ActorManager::Instance().Create<ParticlePang>(
		static_cast<int>(GameBase::Layer::Particle)
		);

	pParticle->SetActive(false);
	pParticle->SetPivot(0.f, 0.f);
	pParticle->SetOriginBlockSize(originSize);

	return pParticle;
}
