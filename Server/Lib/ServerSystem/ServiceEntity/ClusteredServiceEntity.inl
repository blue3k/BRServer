////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Component class implementation
//	
//
////////////////////////////////////////////////////////////////////////////////



template<class ServiceType>
HRESULT ClusteredServiceEntity::GetService( ServiceType* &pService )
{
	HRESULT hr = S_SYSTEM_OK;
	ServerServiceInformation* pServiceInfo = nullptr;

	if( GetMasterUID() == 0 )
		return E_SVR_CLUSTER_NOMASTER;

	svrChk( FindService( GetMasterUID(),  pServiceInfo ) );

	pService = pServiceInfo->GetService<ServiceType>();

Proc_End:


	return hr;
}





