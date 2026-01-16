#pragma once
#include <random>

namespace Sexy
{
	// It's a custom MTRand class implementation
	// It's designed as an utility class rather than a recreation of the layout
	class MTRand
	{
	public:
		std::mt19937 m_mti;
		int m_seed;

		MTRand()
		{
			m_seed = 4357;
			m_mti(m_seed);
		}

		MTRand(uint seed)
		{
			m_seed = seed;
			m_mti(m_seed);
		}

		int RandomIntInRange(int theMin, int theMax)
		{
			return std::uniform_int_distribution<>(theMin, theMax)(m_mti);
		}

		float RandomFloatInRange(float theMin, float theMax)
		{
			return std::uniform_real_distribution<float>(theMin, theMax)(m_mti);
		}
	};
}