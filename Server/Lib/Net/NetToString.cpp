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
#include "Net/NetUtil.h"


namespace BR {
	

	template<>
	Result ToString( char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionState& Data, int Option )
	{
		unused(Option);
		return ToStringEnum( pBuff, iBuffLen, Data );
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_in6& Data, int Option)
	{
		unused(Option);
		char ipstr[INET6_ADDRSTRLEN] = "";
		inet_ntop(Data.sin6_family, (void*)&Data.sin6_addr, ipstr, sizeof ipstr);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, ipstr)))
		//if (!(ToString(pBuff, iBuffLen, ipstr, 0)))
			return ResultCode::FAIL;

		if( !( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
			return ResultCode::FAIL;
		
		if( !( _IToA( (UINT32)ntohs(Data.sin6_port), pBuff, iBuffLen, 10, -1 ) ) )
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString(char*& pBuff, INT& iBuffLen, const sockaddr_storage& Data, int Option)
	{
		unused(Option);
		NetAddress addr;

		Net::SockAddr2Addr(Data, addr);

		if (!(StrUtil::StringCpyEx(pBuff, iBuffLen, addr.strAddr)))
			return ResultCode::FAIL;

		if( !( StrUtil::StringCpyEx( pBuff, iBuffLen, ":" ) ) )
			return ResultCode::FAIL;
		
		if( !( _IToA( (UINT32)addr.usPort, pBuff, iBuffLen, 10, -1 ) ) )
			return ResultCode::FAIL;

		return ResultCode::SUCCESS;
	}


	template<>
	Result ToString( char*& pBuff, INT& iBuffLen, const Net::IConnection::ConnectionInformation& Data, int Option )
	{
		unused(Option);
		if( !( StrUtil::StringCpyEx( pBuff, iBuffLen, "Local:" ) ) )
			return ResultCode::FAIL;

		if( !( ToString( pBuff, iBuffLen, Data.Local, 0) ) )
			return ResultCode::FAIL;

		if( !( StrUtil::StringCpyEx( pBuff, iBuffLen, "Remote:" ) ) )
			return ResultCode::FAIL;

		if( !( ToString( pBuff, iBuffLen, Data.Remote, 0) ) )
			return ResultCode::FAIL;


		return ResultCode::SUCCESS;
	}

		
	template<>
	Result ToString( char*& pBuff, INT& iBuffLen, const NetClass& Data, int Option )
	{
		unused(Option);
		return ToStringEnum( pBuff, iBuffLen, Data );
	}


} // namespace BR


