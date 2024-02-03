#include "PCH.h"
#include "ParticlePang.h"

using namespace DX;
using namespace SpiritPang;

#pragma warning(push)
#pragma warning(disable:26812)
ParticlePang::ParticlePang() :
	AnimatedActor(L"Assets/Particles.png", L"Assets/Particles.json", L"Assets/AnimDestroyParticle.json"),
	m_type{ ParticleType::None }
{
	SetOnEnd(AnimatedActor::OnEnd::Disable);
}
#pragma warning(pop)

void ParticlePang::SetAnimation(ParticleType type)
{
	if (m_type == type)
	{
		return;
	}
	
	m_type = type;
	switch (m_type)
	{
		case ParticlePang::Disappear:
			AnimatedActor::SetAnimation(L"Disappear");
			SetSize(m_originSize * 1.5f);
			break;
		case ParticlePang::BombAttack:
			AnimatedActor::SetAnimation(L"BombAttack");
			SetSize(m_originSize * 3.5f);
			break;
		case ParticlePang::BombSplash:
			AnimatedActor::SetAnimation(L"BombSplash");
			SetSize(m_originSize * 1.2f);
			break;
		default:
			break;
	}
}
