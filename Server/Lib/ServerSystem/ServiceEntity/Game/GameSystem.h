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
#include "Memory/SFMemoryPool.h"
#include "Component/BrComponent.h"


namespace SF {
namespace Svr {
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	GameSystem
	//

	template< class OwnerEntity, class SystemClass >
	class GameSystem : public Component
	{
	public:

	private:
		// Owner Entity
		OwnerEntity&			m_OwnerEntity;

		// Modified IDs
		std::unordered_set<uint>	m_ModifiedIDs;

	protected:

	public:

		// Constructor
		GameSystem( OwnerEntity* pOwner );
		virtual ~GameSystem();

		// Get Owner
		inline OwnerEntity& GetOwner();
		
		IHeap& GetHeap() { return GetOwner().GetHeap(); }

		//////////////////////////////////////////////////////////////////////////////
		//
		//	Modified IDs
		//

		// Initialize modified
		inline void ClearModifiedInfo();

		// set modified quest
		inline void SetModified( uint uiID );

		inline bool IsModified( uint uiID );

		// Get Modified mission clear
		std::unordered_set<uint>& GetModifiedList();

		// Initialize server component
		virtual Result InitializeComponent() { ClearModifiedInfo(); return Component::InitializeComponent(); }
		// Terminate server component
		virtual void TerminateComponent() { Component::TerminateComponent(); }

		// Update system
		virtual Result UpdateSystem() { return ResultCode::SUCCESS; }

	};




#include "GameSystem.inl"



}; // namespace Svr
}; // namespace SF




