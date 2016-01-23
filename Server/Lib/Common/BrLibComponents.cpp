////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : XML Parser Handler
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/Utility.h"
#include "Common/BrLibComponents.h"



namespace BR {

	LibComponentManager LibComponentManager::stm_Instance;
	LibComponentManager& LibComponentManager::GetInstance()
	{
		return stm_Instance;
	}

	LibComponentManager::LibComponentManager()
	{

	}

	LibComponentManager::~LibComponentManager()
	{

	}

	// Clear components
	void LibComponentManager::ClearComponents()
	{
		for (size_t iComponent = 0; iComponent < (size_t)LibCompoentIDs::Max; iComponent++)
		{
			auto pComponent = RemoveComponent((UINT)iComponent);
			Util::SafeDelete(pComponent);
		}
	}

} // namespace BR

