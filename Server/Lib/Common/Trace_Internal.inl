////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : debug trace message definitions. 
//
////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////
//
// Trace Output Module
//

void TraceOutModule::NewInstance()
{
	stm_pInstance = new TraceOutModule;
}

void TraceOutModule::SetInstance( TraceOutModule *pInstance )
{
	stm_pInstance = pInstance;
}

TraceOutModule* TraceOutModule::GetInstance()
{
	return stm_pInstance;
}

