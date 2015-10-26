////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : utility
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/BrRandom.h"
#include "Common/Trace.h"







namespace BR {
namespace Util {


	// Global random generator
	RandomGen Random;



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Random generator ( Combination Generator )
	//


	// Constructor
	// uiRandMax : max random alue
	// puiSeeds : initial seeds must have NUM_SEED count element, if NULL then initialize by STD rand
	RandomGen::RandomGen( UINT uiRandMax, UINT *puiSeeds )
		:m_uiCurIdx(0),
		m_uiRandMax(uiRandMax)
	{
		Srand( puiSeeds );
	}

	// Destructor
	RandomGen::~RandomGen()
	{
	}

	// Change initial seed and reinitialize
	// puiSeeds : initial seeds must have NUM_SEED count element
	void RandomGen::Srand( UINT *puiSeeds )
	{
		if( puiSeeds )
		{
			for( UINT uiSeed = 0; uiSeed < NUM_SEED; uiSeed++ )
				m_uiSeeds[uiSeed] =  puiSeeds[uiSeed];
		}
		else
		{
			for( UINT uiSeed = 0; uiSeed < NUM_SEED; uiSeed++ )
				m_uiSeeds[uiSeed] =  (((UINT)rand() << 16) | (UINT)rand()) % m_uiRandMax;
		}
	}

	// Generate new random number
	UINT RandomGen::Rand()
	{
		auto curIdx = m_uiCurIdx.fetch_add(1, std::memory_order_relaxed) % NUM_SEED;

		UINT uiNewRand = (m_uiSeeds[(curIdx - 24) % NUM_SEED] + m_uiSeeds[(curIdx - 55) % NUM_SEED]) % m_uiRandMax;

		m_uiSeeds[curIdx] = uiNewRand;


		return uiNewRand;
	}



	////////////////////////////////////////////////////////////////////////////////
	//
	//	Linear Random generator ( Linear Generator )
	//


	// Constructor
	// uiRandMax : max random alue
	// uiSeed : initial seed, if 0 then initialize by STD rand
	RandomGenLinear::RandomGenLinear( UINT uiRandMax, UINT uiSeed )
		:m_uiRandMax(uiRandMax)
	{
		if( m_uiRandMax <= (1<<15) )
		{
			// MS default
			m_uiMul = 214013L;
			m_uiAdd = 2531011L;
			m_uiMod = 65535;
		}
		else if( m_uiRandMax <= (1<<16) )
		{
			m_uiMul = 47486;
			m_uiAdd = 0;
			m_uiMod = 1<<16;
		}
		else //if( m_uiRandMax <= (0x7FFFFFFFL) ) // 32bit maximum
		{
			m_uiMul = 16807;
			m_uiAdd = 0;
			m_uiMod = 0xFFFFFFFFL;
		}

		Srand( uiSeed );
	}

	// Destructor
	RandomGenLinear::~RandomGenLinear()
	{
	}


	// GetCurrentSeedValue;
	UINT RandomGenLinear::GetSeed()
	{
		return m_uiSeed;
	}

	// Change initial seed and reinitialize
	// uiSeed : initial seed, if 0 then initialize by STD rand
	void RandomGenLinear::Srand( UINT uiSeed )
	{
		if( uiSeed > 0 )
		{
			m_uiSeed =  uiSeed;
		}
		else
		{
			m_uiSeed =  (((UINT)rand() << 16) | (UINT)rand()) % m_uiRandMax;
		}
	}

	// Generate new random number
	UINT RandomGenLinear::Rand()
	{
		m_uiSeed = (m_uiSeed * m_uiMul + m_uiAdd) % m_uiMod;

		return m_uiSeed % m_uiRandMax;
	}





};	// namespace Util
} // namespace BR

