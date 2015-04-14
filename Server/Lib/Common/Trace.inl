////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 Madk
// 
// Author : KyungKun Ko
//
// Description : Server constants definitions. 
//
////////////////////////////////////////////////////////////////////////////////


namespace BR {
namespace Trace {


	////////////////////////////////////////////////////////////////////////////////
	//
	// Trace Log Module
	//

	bool TraceModule::CheckTrace( UINT trcMask )
	{
		UINT uiTrace = m_uiTraceMask&trcMask;
		if( uiTrace == 0 ) 
			return false;

		return true;
	}

	UINT TraceModule::GetTraceMask()
	{
		return m_uiTraceMask;
	}

	const char *TraceModule::GetName()
	{
		return m_szName;
	}

	const char *TraceModule::GetNameTag()
	{
		return m_szNameTag;
	}


}; // namespace Trace
} // BR

