////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Common/Typedefs.h"
#include "Common/Synchronization.h"



namespace BR {
namespace Util {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Random generator ( Combination Generator )
	//

	class RandomGen
	{
	public:
		// Constants definitions
		enum { 
			NUM_SEED = 1024, 
			GLOBAL_MAXIMUM = 65535
		};

	private:
		// Seeds for generator
		UINT m_uiSeeds[NUM_SEED];
		std::atomic<UINT> m_uiCurIdx;
		UINT m_uiRandMax;

	public:
		// Constructor
		// uiRandMax : max random alue
		// puiSeeds : initial seeds must have NUM_SEED count element, if NULL then initialize by STD rand
		RandomGen( UINT uiRandMax = RandomGen::GLOBAL_MAXIMUM, UINT *puiSeeds = NULL );

		// Destructor
		~RandomGen();

		// Change initial seed and reinitialize
		// puiSeeds : initial seeds must have NUM_SEED count element
		void Srand( UINT *puiSeeds );

		// Generate new random number
		UINT Rand();

		// Generate new random number
		inline UINT Rand( UINT iMax );

		// Generate new random number
		inline INT Rand( INT iMin, INT iMax );
	};


	////////////////////////////////////////////////////////////////////////////////
	//
	//	Simple Random generator ( linear Generator )
	//

	class RandomGenLinear
	{
	public:

	private:
		// Seeds for generator
		UINT m_uiSeed;
		UINT m_uiMul, m_uiAdd, m_uiMod;
		UINT m_uiRandMax;

	public:
		// Constructor
		// uiRandMax : max random alue
		// uiSeed : initial seed, if 0 then initialize by STD rand
		RandomGenLinear( UINT uiRandMax = RandomGen::GLOBAL_MAXIMUM, UINT uiSeed = 0 );

		// Destructor
		~RandomGenLinear();

		// GetCurrentSeedValue;
		UINT GetSeed();

		// Change initial seed and reinitialize
		// uiSeed : initial seed, if 0 then initialize by STD rand
		void Srand( UINT uiSeed );

		// Generate new random number
		UINT Rand();

		// Generate new random number
		inline UINT Rand( UINT iMax );

		// Generate new random number
		inline INT Rand( INT iMin, INT iMax );
	};

	// Global random generator
	extern RandomGen Random;



	
#include "BrRandom.inl"


}; // namespace Util
} // namespace BR


