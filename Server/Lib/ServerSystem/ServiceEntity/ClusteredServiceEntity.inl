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
Result ClusteredServiceEntity::GetService( ServiceType* &pService )
{
	Result hr = ResultCode::SUCCESS;
	ServerServiceInformation* pServiceInfo = nullptr;

	if( GetMasterUID() == 0 )
		return ResultCode::E_CLUSTER_NOMASTER;

	svrChk( FindService( GetMasterUID(),  pServiceInfo ) );

	pService = pServiceInfo->GetService<ServiceType>();

Proc_End:


	return hr;
}





