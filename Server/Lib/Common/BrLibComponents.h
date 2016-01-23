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
		EnvSetting,
		ThirdParty,
		Trace,
		MemoryPoolManager,
		Timer,
		Max,
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Component career base class
	//


	class LibComponentManager : public ComponentCarrier<(size_t)LibCompoentIDs::Max>
	{
	private:

		static LibComponentManager stm_Instance;

		LibComponentManager();
		~LibComponentManager();

	public:

		static LibComponentManager& GetInstance();

		virtual void ClearComponents() override;


	};




}; // namespace BR




