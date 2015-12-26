////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class definition
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "Common/Typedefs.h"
#include "Common/BrComponent.h"


namespace BR {


	enum class LibCompoentIDs : UINT8
	{
		Trace,
		MemoryPoolManager,
		Timer,
		Max,
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Component career base class
	//


	class LibComponentCarrier : public ComponentCarrier<(size_t)LibCompoentIDs::Max>
	{
	public:

		LibComponentCarrier();
		~LibComponentCarrier();

		virtual void ClearComponents() override;
	};




}; // namespace BR




