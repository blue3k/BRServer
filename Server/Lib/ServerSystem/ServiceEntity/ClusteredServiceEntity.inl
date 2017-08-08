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
		return ResultCode::E_SVR_CLUSTER_NOMASTER;

	if (!FindService(GetMasterUID(), pServiceInfo))
		return ResultCode::E_SVR_SERVICE_FAILED;

	pService = pServiceInfo->GetService<ServiceType>();

Proc_End:


	return hr;
}





