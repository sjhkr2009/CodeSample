#include "PCH.h"
#include "Spirit.h"
#include "ParticlePang.h"
#pragma once

namespace SpiritPang
{
	class SpiritFactory
	{
	public:
		static Spirit* CreateSpirit(int type, float size);
		static ParticlePang* CreateDestroyParticle(float originSize);
	};
}

