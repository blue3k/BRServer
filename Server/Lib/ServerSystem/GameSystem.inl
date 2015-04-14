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



///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Remote Entity class
//

// Constructor
template< class OwnerEntity, class SystemClass >
GameSystem<OwnerEntity,SystemClass>::GameSystem( OwnerEntity* pOwner )
	:m_OwnerEntity(*pOwner)
	,Component(SystemClass::ComponentID)
{
}

template< class OwnerEntity, class SystemClass >
GameSystem<OwnerEntity,SystemClass>::~GameSystem()
{
}

// Get Owner
template< class OwnerEntity, class SystemClass >
OwnerEntity& GameSystem<OwnerEntity,SystemClass>::GetOwner()
{
	return m_OwnerEntity;
}




//////////////////////////////////////////////////////////////////////////////
//
//	Modified IDs
//

// Initialize modified
template< class OwnerEntity, class SystemClass >
void GameSystem<OwnerEntity,SystemClass>::ClearModifiedInfo()
{
	m_ModifiedIDs.clear();
}


// set modified quest
template< class OwnerEntity, class SystemClass >
void GameSystem<OwnerEntity,SystemClass>::SetModified( UINT uiID )
{
	m_ModifiedIDs.insert( uiID );
}


template< class OwnerEntity, class SystemClass >
bool GameSystem<OwnerEntity,SystemClass>::IsModified( UINT uiID )
{
	return m_ModifiedIDs.find( uiID ) != m_ModifiedIDs.end();
}



// Get Modified mission clear
template< class OwnerEntity, class SystemClass >
std::unordered_set<UINT>& GameSystem<OwnerEntity,SystemClass>::GetModifiedList()
{
	return m_ModifiedIDs;
}



