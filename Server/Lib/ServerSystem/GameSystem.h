////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : GameSystem template
//	
//
////////////////////////////////////////////////////////////////////////////////


#pragma once


#include "SFTypedefs.h"
#include "Memory/MemoryPool.h"
#include "Common/BrComponent.h"


namespace BR {
namespace Svr {
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameSystem
	//

	template< class OwnerEntity, class SystemClass >
	class GameSystem : public Component, public BR::MemoryPoolObject<SystemClass>
	{
	public:

	private:
		// Owner Entity
		OwnerEntity&			m_OwnerEntity;

		// Modified IDs
		std::unordered_set<UINT>	m_ModifiedIDs;

	protected:

	public:

		// Constructor
		GameSystem( OwnerEntity* pOwner );
		virtual ~GameSystem();

		// Get Owner
		inline OwnerEntity& GetOwner();
		

		//////////////////////////////////////////////////////////////////////////////
		//
		//	Modified IDs
		//

		// Initialize modified
		inline void ClearModifiedInfo();

		// set modified quest
		inline void SetModified( UINT uiID );

		inline bool IsModified( UINT uiID );

		// Get Modified mission clear
		std::unordered_set<UINT>& GetModifiedList();

		// Initialize server component
		virtual Result InitializeComponent() { ClearModifiedInfo(); return Component::InitializeComponent(); }
		// Terminate server component
		virtual void TerminateComponent() { Component::TerminateComponent(); }

		// Update system
		virtual Result UpdateSystem() { return ResultCode::SUCCESS; }

	};




#include "GameSystem.inl"



}; // namespace Svr
}; // namespace BR




