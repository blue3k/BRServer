////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves 
// 
// Author : KyungKun Ko
//
// Description : Network type tostring 
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Net/NetToString.h"


namespace BR {
	

	template<>
	HRESULT ToString( char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionState& Data, int Option )
	{
		unused(Option);
		return ToStringEnum( pBuff, iBuffLen, Data );
	}


	template<>
	HRESULT ToString(char*& pBuff, INT& iBuffLen, const sockaddr_in6& Data, int Option)
	{
		unused(Option);
		char ipstr[INET6_ADDRSTRLEN] = "";
		inet_ntop(Data.sin6_family, (void*)&Data.sin6_addr, ipstr, sizeof ipstr);

		if (FAILED(StrUtil::StringCpyEx(pBuff, iBuffLen, ipstr)))
		//if (FAILED(ToString(pBuff, iBuffLen, ipstr, 0)))
			return E_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
			return E_FAIL;
		
		if( FAILED( _IToA( (UINT32)ntohs(Data.sin6_port), pBuff, iBuffLen, 10, -1 ) ) )
			return E_FAIL;

		return S_OK;
	}



	template<>
	HRESULT ToString( char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionInformation& Data, int Option )
	{
		unused(Option);
		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "Local:" ) ) )
			return E_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.Local, 0) ) )
			return E_FAIL;

		if( FAILED( StrUtil::StringCpyEx( pBuff, iBuffLen, "Remote:" ) ) )
			return E_FAIL;

		if( FAILED( ToString( pBuff, iBuffLen, Data.Remote, 0) ) )
			return E_FAIL;


		return S_OK;
	}

		
	template<>
	HRESULT ToString( char*& pBuff, INT& iBuffLen, const NetClass& Data, int Option )
	{
		unused(Option);
		return ToStringEnum( pBuff, iBuffLen, Data );
	}


} // namespace BR


