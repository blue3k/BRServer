﻿////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Configuration
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "ServerSystem/SvrConfig.h"
#include "ServerSystem/SvrTrace.h"



namespace BR {
namespace Svr {
namespace Config
{
	// static instance for config data
	static ConfigData __ConfigData;

	///////////////////////////////////////////////////////////////////////////////

	NetSocket::NetSocket()
		: XML::DOMElement("NetSocket"),
		Port(0)
	{
	}

	// for parsing
	bool NetSocket::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "IP" ) {
			IP = value;
		}
		else if( name == "Port" ) {
			Port = (UINT)atoi( value.c_str() );
		}
		else {
			return XML::DOMElement::SetAttributeValue( name, value );
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	PublicNetSocket::PublicNetSocket()
		: XML::DOMElement("PublicNetSocket")
		, Port(0)
		, MaxConnection(1000000)
	{
	}

	// for parsing
	bool PublicNetSocket::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "IPV4") {
			IPV4 = value;
		}
		else if (name == "IPV6") {
			IPV6 = value;
		}
		else if (name == "Port") {
			Port = (UINT)atoi(value.c_str());
		}
		else if (name == "MaxConnection") {
			MaxConnection = (UINT)atoi(value.c_str());
		}
		else {
			return XML::DOMElement::SetAttributeValue(name, value);
		}
		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	DBInstance::DBInstance()
		: XML::DOMElement("DBInstance")
	{
	}

	// for parsing
	bool DBInstance::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "InstanceName" )
		{
			InstanceName = value;
		}
		else if( name == "ConnectionString" ) {
			ConnectionString = value;
		}
		else if( name == "UserID" ) {
			UserID = value;
		}
		else if( name == "Password" ) {
			Password = value;
		}
		else {
			return XML::DOMElement::SetAttributeValue( name, value );
		}

		return true;
	}
	

	///////////////////////////////////////////////////////////////////////////////

	DBClusterInstance::DBClusterInstance()
		: XML::DOMElement("DBClusterInstance")
	{
	}

	// for parsing
	bool DBClusterInstance::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "DBInstanceName" ) {
			DBInstanceName = value;
		}
		else if( name == "DBName" ) {
			DBName = value;
		}
		else if( name == "PartitioningID" ) {
			PartitioningID = (UINT)atoi( value.c_str() );;
		}
		else {
			return XML::DOMElement::SetAttributeValue( name, value );
		}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	DBSource::DBSource()
		: XML::DOMElement("DBSource")
	{
	}

	// for parsing
	bool DBSource::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "DBInstanceName") {
			DBInstanceName = value;
		}
		else if (name == "DBName") {
			DBName = value;
		}
		else {
			return XML::DOMElement::SetAttributeValue(name, value);
		}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	DBCluster::DBCluster()
		: XML::DOMElement("DBCluster")
	{
	}

	void DBCluster::AddChild( DOMElement *pChild )
	{
		AssertRel( pChild->GetTypeName() == "DBClusterInstance" );

		DBMembers.push_back( (DBClusterInstance*)pChild );

		XML::DOMElement::AddChild( pChild );
	}

	// for parsing
	bool DBCluster::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "ClusterType" ) {
			if( value == "Sharding" )
				ClusterType = DBClusterType::Sharding;
			else
				ClusterType = DBClusterType::Normal;
		}
		else if( name == "PartitioningCount" ) {
			PartitioningCount = atoi( value.c_str() );
		}
		else {
			return XML::DOMElement::SetAttributeValue( name, value );
		}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	DBServer::DBServer()
		: XML::DOMElement("DBServer")
	{
	}

	// for parsing
	bool DBServer::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "ConnectionString" ) {
			ConnectionString = value;
		}
		else if( name == "UserID" ) {
			UserID = value;
		}
		else if( name == "Password" ) {
			Password = value;
		}
		else if( name == "DBName" ) {
			DBName = value;
		}
		else if( name == "WorkerCount" ) {
			WorkerCount = (ULONG)atoi( value.c_str() );
		}
		else {
			return XML::DOMElement::SetAttributeValue( name, value );
		}

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	static const struct ModuleClusterIDDef {
		const char* Name;
		ClusterID ModuleClusterID;
	} g_ModuleClusterIDValues[] =
	{
		{ "ModGamePartyManager", ClusterID::GamePartyManager },
		{ "ModMonitoring", ClusterID::Monitoring },
		{ "ModLogin", ClusterID::Login },
		{ "ModGame", ClusterID::Game },
		{ "ModInstanceManager", ClusterID::GameInstanceManager },
		{ "ModMatching_Game_4", ClusterID::Matching_Game_4 },
		{ "ModMatching_Game_8", ClusterID::Matching_Game_8 },
		{ "ModMatchingQueue_Game_4x1", ClusterID::MatchingQueue_Game_4x1 },
		{ "ModMatchingQueue_Game_4x2", ClusterID::MatchingQueue_Game_4x2 },
		{ "ModMatchingQueue_Game_4x3", ClusterID::MatchingQueue_Game_4x3 },
		{ "ModMatchingQueue_Game_4x1S", ClusterID::MatchingQueue_Game_4x1S },
		{ "ModMatchingQueue_Game_4x1W", ClusterID::MatchingQueue_Game_4x1W },
		{ "ModMatchingQueue_Game_8x1", ClusterID::MatchingQueue_Game_8x1 },
		{ "ModMatchingQueue_Game_8x2", ClusterID::MatchingQueue_Game_8x2 },
		{ "ModMatchingQueue_Game_8x3", ClusterID::MatchingQueue_Game_8x3 },
		{ "ModMatchingQueue_Game_8x4", ClusterID::MatchingQueue_Game_8x4 },
		{ "ModMatchingQueue_Game_8x5", ClusterID::MatchingQueue_Game_8x5 },
		{ "ModMatchingQueue_Game_8x6", ClusterID::MatchingQueue_Game_8x6 },
		{ "ModMatchingQueue_Game_8x7", ClusterID::MatchingQueue_Game_8x7 },
		{ "ModMatchingQueue_Game_8x1W", ClusterID::MatchingQueue_Game_8x1W },
		{ "ModMatchingQueue_Game_8x1S", ClusterID::MatchingQueue_Game_8x1S },
		{ "ModPurchaseValidateGoogle", ClusterID::PurchaseValidateGoogle },
		{ "ModPurchaseValidateIOS", ClusterID::PurchaseValidateIOS },
	};

	static std::unordered_map<std::string, const ModuleClusterIDDef*> g_ModuleClusterIDMap;

	ModuleBase::ModuleBase()
		: XML::DOMElement("ModuleBase")
		, m_ModuleClusterID(ClusterID::Max)
	{
		if (g_ModuleClusterIDMap.size() == 0)
		{
			for (int iValue = 0; iValue < countof(g_ModuleClusterIDValues); iValue++)
			{
				g_ModuleClusterIDMap.insert(std::make_pair(g_ModuleClusterIDValues[iValue].Name, &g_ModuleClusterIDValues[iValue]));
			}
		}
	}

	ModuleBase::ModuleBase(const char* name, ClusterID clusterID)
		: XML::DOMElement(name)
		, m_ModuleClusterID(clusterID)
	{
		if (g_ModuleClusterIDMap.size() == 0)
		{
			for (int iValue = 0; iValue < countof(g_ModuleClusterIDValues); iValue++)
			{
				g_ModuleClusterIDMap.insert(std::make_pair(g_ModuleClusterIDValues[iValue].Name, &g_ModuleClusterIDValues[iValue]));
			}
		}
	}

	void ModuleBase::SetName(const char* Name)
	{
		XML::DOMElement::SetName(Name);

		auto itFound = g_ModuleClusterIDMap.find(Name);
		AssertRel(itFound != g_ModuleClusterIDMap.end());

		SetModuleClusterID(itFound->second->ModuleClusterID);
	}

	// for parsing
	bool ModuleBase::SetAttributeValue(const std::string& name, const std::string& value)
	{
		return XML::DOMElement::SetAttributeValue(name, value);
	}

	///////////////////////////////////////////////////////////////////////////////

	ModuleSimple::ModuleSimple()
		: ModuleBase("ModuleSimple", ClusterID::Max)
	{
	}

	// for parsing
	bool ModuleSimple::SetAttributeValue(const std::string& name, const std::string& value)
	{
		//if (name == "IPV4") {
		//	IPV4 = value;
		//}
		//else if (name == "MaxConnections") {
		//	MaxConnections = (UINT)atoi(value.c_str());
		//}
		//else {
			return ModuleBase::SetAttributeValue(name, value);
		//}
		//return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModuleMatching::ModuleMatching()
		: ModuleBase("ModuleMatching", ClusterID::Matching_Game_4)
		, UseBot(true)
	{
	}

	// for parsing
	bool ModuleMatching::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "UseBot") {
			UseBot = value == "true" || value == "1";
		}
		else {
			return ModuleBase::SetAttributeValue(name, value);
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModuleLogin::ModuleLogin()
		: ModuleBase("ModuleLogin", ClusterID::Login)
		, NetPublic(nullptr)
	{
	}

	void ModuleLogin::AddChild(DOMElement *pChild)
	{
		XML::DOMElement::AddChild(pChild);

		Assert(pChild);

		if (pChild->GetName() == "NetPublic") {
			NetPublic = dynamic_cast<PublicNetSocket*>(pChild);
		}
	}

	// for parsing
	bool ModuleLogin::SetAttributeValue(const std::string& name, const std::string& value)
	{
		//if (name == "IPV4") {
		//	IPV4 = value;
		//}
		//else if (name == "MaxConnections") {
		//	MaxConnections = (UINT)atoi(value.c_str());
		//}
		//else {
		return ModuleBase::SetAttributeValue(name, value);
		//}
		//return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModuleGame::ModuleGame()
		: ModuleBase("ModuleGame", ClusterID::Game)
		, NetPublic(nullptr)
	{
	}

	void ModuleGame::AddChild(DOMElement *pChild)
	{
		XML::DOMElement::AddChild(pChild);

		Assert(pChild);

		if (pChild->GetName() == "NetPublic") {
			NetPublic = dynamic_cast<PublicNetSocket*>(pChild);
		}
	}

	// for parsing
	bool ModuleGame::SetAttributeValue(const std::string& name, const std::string& value)
	{
		//if (name == "GameID") {
		//	if(value == "Conspiracy")		m_GameID = GameID::Conspiracy;
		//	else if (value == "Game")		m_GameID = GameID::Game;
		//	else
		//	{
		//		Assert(false);
		//	}
		//}
//		else {
			return ModuleBase::SetAttributeValue(name, value);
	//	}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModuleGameInstance::ModuleGameInstance()
		: ModuleBase("ModuleGameInstance", ClusterID::GameInstanceManager)
	{
	}

	void ModuleGameInstance::AddChild(DOMElement *pChild)
	{
		XML::DOMElement::AddChild(pChild);
	}

	// for parsing
	bool ModuleGameInstance::SetAttributeValue(const std::string& name, const std::string& value)
	{
		//if (name == "IPV4") {
		//	IPV4 = value;
		//}
		//else if (name == "MaxConnections") {
		//	MaxConnections = (UINT)atoi(value.c_str());
		//}
		//else {
		return ModuleBase::SetAttributeValue(name, value);
		//}
		//return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModulePurchaseValidateGoogle::ModulePurchaseValidateGoogle()
		: ModuleBase("ModulePurchaseValidateGoogle", ClusterID::PurchaseValidateGoogle)
	{
	}

	// for parsing
	bool ModulePurchaseValidateGoogle::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "Account") {
			Account = value;
		}
		else if (name == "P12KeyFile") {
			P12KeyFile = value;
		}
		else if (name == "AuthScopes") {
			AuthScopes = value;
		}
		else {
			return ModuleBase::SetAttributeValue(name, value);
		}

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ModulePurchaseValidateIOS::ModulePurchaseValidateIOS()
		: ModuleBase("ModulePurchaseValidateIOS", ClusterID::PurchaseValidateIOS)
	{
	}

	// for parsing
	bool ModulePurchaseValidateIOS::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "URL") {
			URL = value;
		}
		else if (name == "AltURL") {
			AltURL = value;
		}
		else {
			return ModuleBase::SetAttributeValue(name, value);
		}

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////

	ServerComponent::ServerComponent(const char* typeName)
		: XML::DOMElement(typeName)
	{
	}

	///////////////////////////////////////////////////////////////////////////////

	ServerComponentGoogle::ServerComponentGoogle()
		: ServerComponent("ServerComponentGoogle")
	{
	}

	// for parsing
	bool ServerComponentGoogle::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "Account") {
			Account = value;
		}
		else if (name == "P12KeyFile") {
			P12KeyFile = value;
		}
		else if (name == "AuthScopes") {
			AuthScopes = value;
		}
		else {
			return ServerComponent::SetAttributeValue(name, value);
		}

		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	ServerComponentIOS::ServerComponentIOS()
		: ServerComponent("ServerComponentIOS")
	{
	}

	// for parsing
	bool ServerComponentIOS::SetAttributeValue(const std::string& name, const std::string& value)
	{
		if (name == "URL") {
			URL = value;
		}
		else {
			return ServerComponent::SetAttributeValue(name, value);
		}

		return true;
	}



	///////////////////////////////////////////////////////////////////////////////

	void GameClusterInfo::AddChild( DOMElement *pChild )
	{
		XML::DOMElement::AddChild( pChild );

		Assert( pChild );

		if( pChild->GetName() == "GameDB" ) {
			GameDB = dynamic_cast<DBCluster*>(pChild);
		}
		else if( pChild->GetName() == "GameTransactionLogDB" ) {
			GameTransactionLogDB = dynamic_cast<DBCluster*>(pChild);
		}
		else if( pChild->GetName() == "Game" ) {
			PublicServer* pServer = dynamic_cast<PublicServer*>(pChild);
			Assert(pServer);
			Games.push_back( pServer );
		}
		else if( pChild->GetName() == "GameInstance" ) {
			GenericServer* pServer = dynamic_cast<GenericServer*>(pChild);
			Assert(pServer);
			GameInstancess.push_back( pServer );
		}
		else if( pChild->GetName() == "GameMatchingQueue" ) {
			GenericServer* pServer = dynamic_cast<GenericServer*>(pChild);
			Assert(pServer);
			GameMatchingQueues.push_back( pServer );
		}
		else if( pChild->GetName() == "GameMatching" ) {
			GenericServer* pServer = dynamic_cast<GenericServer*>(pChild);
			Assert(pServer);
			GameMatchings.push_back( pServer );
		}
	}

	// for parsing
	bool GameClusterInfo::SetAttributeValue( const std::string& attName, const std::string& value )
	{
		if( attName == "ClusterID" ) {
			ClusterID = (UINT)atoi( value.c_str() );
		}
		else if( attName == "Name" ) {
			ClusterName = value;
		}
		else if( attName == "GameID" ) {
			if (value == "Conspiracy")		m_GameID = GameID::Conspiracy;
			else if (value == "Game")		m_GameID = GameID::Game;
			else
			{
				Assert(false);
			}
		}
		else {
			return XML::DOMElement::SetAttributeValue( attName, value );
		}
		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	GenericServer::GenericServer()
		: UID(0),
		//ClusterID(0),
		PrivateEntityControlCount(4),
		EntityControlCount(4)
	{
	}

	void GenericServer::AddChild( DOMElement *pChild )
	{
		XML::DOMElement::AddChild( pChild );

		Assert( pChild );

		if( pChild->GetName() == "NetPrivate" )
		{
			NetPrivate = dynamic_cast<NetSocket*>(pChild);
		}
		else
		{
			auto component  =dynamic_cast<ServerComponent*>(pChild);
			if (component != nullptr)
			{
				ServerComponents.push_back(component);
			}
		}
	}

	// for parsing
	bool GenericServer::SetAttributeValue( const std::string& attName, const std::string& value )
	{
		if( attName == "UID" ) {
			UID = (UINT)atoi( value.c_str() );
		}
		else if( attName == "Name" ) {
			Name = value;
		}
		else if( attName == "EntityControlCount" ) {
			EntityControlCount = (UINT)atoi( value.c_str() );
		}
		else if( attName == "PrivateEntityControlCount" ) {
			PrivateEntityControlCount = (UINT)atoi( value.c_str() );
		}
		else if( attName == "NetIOCount" ) {
			NetIOCount = (UINT)atoi( value.c_str() );
		}
		else
		{
			return XML::DOMElement::SetAttributeValue( attName, value );
		}
		return true;
	}

	// Called when the load done
	bool GenericServer::OnLoadDone()
	{
		bool bSuccess = __ConfigData.ServerByID.insert( std::make_pair(UID,this) ).second;
		Assert(bSuccess);
		if( !bSuccess )
		{
			defTrace( Trace::TRC_ASSERT, "Non Unique ServerID in the configuration : %d", UID );
		}
		return false;
	}

	
	///////////////////////////////////////////////////////////////////////////////

	PublicServer::PublicServer()
		: NetPublic(nullptr)
	{
	}

	void PublicServer::AddChild( DOMElement *pChild )
	{
		GenericServer::AddChild( pChild );

		Assert( pChild );

		if( pChild->GetName() == "NetPublic" )
		{
			NetPublic = dynamic_cast<PublicNetSocket*>(pChild);
			Assert(NetPublic != nullptr);
		}
	}

	// for parsing
	bool PublicServer::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "MaxPublicConnection" )
		{
			MaxPublicConnection = (ULONG)atoi( value.c_str() );
		}
		else
		{
			return GenericServer::SetAttributeValue( name, value );
		}
		return true;
	}


	///////////////////////////////////////////////////////////////////////////////

	//std::unordered_map<std::string,ClusterID> Module::stm_ModuleNameMap;

	//Module::Module()
	//{
	//	if( stm_ModuleNameMap.size() == 0 )
	//	{
	//		stm_ModuleNameMap.insert(std::make_pair("Monitoring", ClusterID::Monitoring));
	//		stm_ModuleNameMap.insert(std::make_pair("GamePartyManager", ClusterID::GamePartyManager ) );
	//		stm_ModuleNameMap.insert(std::make_pair("Matching_Game_4", ClusterID::Matching_Game_4));
	//		stm_ModuleNameMap.insert(std::make_pair("Matching_Game_8", ClusterID::Matching_Game_8 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("Matching_Game_10", ClusterID::Matching_Game_10 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("Matching_Game_12", ClusterID::Matching_Game_12 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_4x1", ClusterID::MatchingQueue_Game_4x1 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_4x2", ClusterID::MatchingQueue_Game_4x2 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_4x3", ClusterID::MatchingQueue_Game_4x3 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_4x1S", ClusterID::MatchingQueue_Game_4x1S));
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_4x1W", ClusterID::MatchingQueue_Game_4x1W));
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x1", ClusterID::MatchingQueue_Game_8x1));
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x2", ClusterID::MatchingQueue_Game_8x2 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x3", ClusterID::MatchingQueue_Game_8x3 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x4", ClusterID::MatchingQueue_Game_8x4 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x5", ClusterID::MatchingQueue_Game_8x5 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x6", ClusterID::MatchingQueue_Game_8x6 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x7", ClusterID::MatchingQueue_Game_8x7 ) );
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x1S", ClusterID::MatchingQueue_Game_8x1S));
	//		stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_8x1W", ClusterID::MatchingQueue_Game_8x1W));
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x1", ClusterID::MatchingQueue_Game_10x1 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x2", ClusterID::MatchingQueue_Game_10x2 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x3", ClusterID::MatchingQueue_Game_10x3 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x4", ClusterID::MatchingQueue_Game_10x4 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x5", ClusterID::MatchingQueue_Game_10x5 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x6", ClusterID::MatchingQueue_Game_10x6 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x7", ClusterID::MatchingQueue_Game_10x7 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x8", ClusterID::MatchingQueue_Game_10x8 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_10x9", ClusterID::MatchingQueue_Game_10x9 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x1", ClusterID::MatchingQueue_Game_12x1 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x2", ClusterID::MatchingQueue_Game_12x2 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x3", ClusterID::MatchingQueue_Game_12x3 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x4", ClusterID::MatchingQueue_Game_12x4 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x5", ClusterID::MatchingQueue_Game_12x5 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x6", ClusterID::MatchingQueue_Game_12x6 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x7", ClusterID::MatchingQueue_Game_12x7 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x8", ClusterID::MatchingQueue_Game_12x8 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x9", ClusterID::MatchingQueue_Game_12x9 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x10", ClusterID::MatchingQueue_Game_12x10 ) );
	//		//stm_ModuleNameMap.insert(std::make_pair("MatchingQueue_Game_12x11", ClusterID::MatchingQueue_Game_12x11 ) );

	//	}
	//}

	//// for parsing
	//bool Module::SetAttributeValue( const std::string& name, const std::string& value )
	//{
	//	if( name == "ModuleID" )
	//	{
	//		auto found = stm_ModuleNameMap.find( value.c_str() );
	//		Assert( found != stm_ModuleNameMap.end() );
	//		ModuleID = found->second;
	//	}
	//	else if( name == "Parameters" )
	//	{
	//		Parameters = value;
	//	}
	//	else
	//	{
	//		return XML::DOMElement::SetAttributeValue( name, value );
	//	}
	//	return true;
	//}
	//


	ModuleServer::ModuleServer()
	{
	}

	void ModuleServer::AddChild( DOMElement *pChild )
	{
		GenericServer::AddChild( pChild );

		Assert( pChild );

		ModuleBase* pModule = dynamic_cast<ModuleBase*>(pChild);
		if(pModule != nullptr)
			Modules.push_back(pModule);
	}

	// for parsing
	bool ModuleServer::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "GameID" )
		{
			if (value == "Conspiracy")		m_GameID = GameID::Conspiracy;
			else if (value == "Game")		m_GameID = GameID::Game;
			else
			{
				Assert(false);
			}
		}
		else
		{
			return GenericServer::SetAttributeValue( name, value );
		}
		return true;
	}
	

	///////////////////////////////////////////////////////////////////////////
	//
	//	Overriding
	//

	void ConfigData::AddChild( DOMElement *pChild )
	{
		XML::DOMElement::AddChild( pChild );

		Assert( pChild );

		if( pChild->GetName() == "DBInstances" )
		{
			auto instance = dynamic_cast<DBInstance*>(pChild);
			DBInstances.insert(std::make_pair(instance->InstanceName,instance));
		}
		else if( pChild->GetName() == "MonitoringServer" )
		{
			MonitoringServer = dynamic_cast<GenericServer*>(pChild);
		}
		else if( pChild->GetName() == "TableDB" )
		{
			TableDB = dynamic_cast<DBSource*>(pChild);
		}
		else if( pChild->GetName() == "AccountDB" )
		{
			AccountDB = dynamic_cast<DBCluster*>(pChild);
		}
		else if( pChild->GetName() == "LoginSessionDB" )
		{
			LoginSessionDB = dynamic_cast<DBCluster*>(pChild);
		}
		else if( pChild->GetName() == "RankingDB" )
		{
			RankingDB = dynamic_cast<DBCluster*>(pChild);
		}
		else if( pChild->GetName() == "LoginServer" )
		{
			auto loginServer = dynamic_cast<PublicServer*>(pChild);
			LoginServers.push_back(loginServer);
		}
		else if( pChild->GetName() == "EntityServer" )
		{
			auto pEntity = dynamic_cast<GenericServer*>(pChild);
			EntityServers.push_back(pEntity);
		}
		else if( pChild->GetName() == "GameCluster" )
		{
			GameCluster = dynamic_cast<GameClusterInfo*>(pChild);
		}
		else if( pChild->GetName() == "ModuleServer" )
		{
			auto pModuleServer = dynamic_cast<ModuleServer*>(pChild);
			ModuleServers.push_back( pModuleServer );
		}
		else
		{
			Assert(false);
		}
	}

	// for parsing
	bool ConfigData::SetAttributeValue( const std::string& name, const std::string& value )
	{
		return XML::DOMElement::SetAttributeValue( name, value );
	}



	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ServerConfigParser decl/impl
	//
	class ServerConfigParser : public BR::XML::XMLParserHandler
	{
	public:
		ServerConfigParser( ConfigData& cfgData )
		{
			RegisterElementCreator( "Servers", [&cfgData]()-> XML::DOMElement* { return &cfgData; } );
			RegisterElementCreator( "DBInstances", []()-> XML::DOMElement* { return new DBInstance; } );
			RegisterElementCreator( "TableDB", []()-> XML::DOMElement* { return new DBSource; } );
			RegisterElementCreator( "AccountDB", []()-> XML::DOMElement* { return new DBCluster; } );
			RegisterElementCreator( "LoginSessionDB", []()-> XML::DOMElement* { return new DBCluster; } );
			RegisterElementCreator( "RankingDB", []()-> XML::DOMElement* { return new DBCluster; } );
			RegisterElementCreator( "DBMember", []()-> XML::DOMElement* { return new DBClusterInstance; } );
			RegisterElementCreator( "MonitoringServer", []()-> XML::DOMElement* { return new GenericServer; } );
			RegisterElementCreator( "EntityServer", []()-> XML::DOMElement* { return new GenericServer; } );
			RegisterElementCreator("ComponentGoogle", []()-> XML::DOMElement* { return new ServerComponentGoogle; });
			RegisterElementCreator("ComponentIOS", []()-> XML::DOMElement* { return new ServerComponentIOS; });

			RegisterElementCreator( "ModuleServer", []()-> XML::DOMElement* { return new ModuleServer; } );

			RegisterElementCreator( "LoginServer", []()-> XML::DOMElement* { return new PublicServer; } );

			RegisterElementCreator( "GameCluster", []()-> XML::DOMElement* { return new GameClusterInfo; } );
			RegisterElementCreator( "GameDB", []()-> XML::DOMElement* { return new DBCluster; } );
			RegisterElementCreator( "GameTransactionLogDB", []()-> XML::DOMElement* { return new DBCluster; } );
			RegisterElementCreator( "Game", []()-> XML::DOMElement* { return new PublicServer; } );
			RegisterElementCreator( "GameInstance", []()-> XML::DOMElement* { return new GenericServer; } );

			RegisterElementCreator( "NetPrivate", []()-> XML::DOMElement* { return new NetSocket; } );
			RegisterElementCreator( "NetPublic", []()-> XML::DOMElement* { return new PublicNetSocket; } );

			RegisterElementCreator("ModGamePartyManager", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMonitoring", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModLogin", []()-> XML::DOMElement* { return new ModuleLogin; });
			RegisterElementCreator("ModGame", []()-> XML::DOMElement* { return new ModuleGame; });
			RegisterElementCreator("ModMatching_Game_4", []()-> XML::DOMElement* { return new ModuleMatching; });
			RegisterElementCreator("ModMatching_Game_8", []()-> XML::DOMElement* { return new ModuleMatching; });
			RegisterElementCreator("ModMatchingQueue_Game_4x1" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_4x2" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_4x3" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_4x1S", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_4x1W", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x1" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x2" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x3" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x4" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x5" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x6" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x7" , []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x1S", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_8x1W", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x1", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x2", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x3", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x4", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x5", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x6", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x7", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x8", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_10x9", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x1", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x2", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x3", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x4", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x5", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x6", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x7", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x8", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x9", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x10", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModMatchingQueue_Game_12x11", []()-> XML::DOMElement* { return new ModuleSimple; });
			RegisterElementCreator("ModPurchaseValidateGoogle", []()-> XML::DOMElement* { return new ModulePurchaseValidateGoogle; });
			RegisterElementCreator("ModPurchaseValidateIOS", []()-> XML::DOMElement* { return new ModulePurchaseValidateIOS; });

			cfgData.ClearConfig();
		}


		HRESULT LoadTable( const char *strFileName )
		{
 			int result = xmlSAXUserParseFile( *this, this, strFileName );

			if (result != 0)
			{
 				// error log
				return E_SYSTEM_FAIL;
			}

			xmlCleanupParser();

			return S_SYSTEM_OK;
		}

	};



	//////////////////////////////////////////////////////////////////////////////
	//
	//	Configuration data
	//

	ConfigData::ConfigData()
	{
	}

	ConfigData::~ConfigData()
	{
		ClearConfig();
	}

	HRESULT ConfigData::ClearConfig()
	{
		DBInstances.clear();

		AccountDB = nullptr;
		GameCluster = nullptr;

		LoginServers.clear();
		EntityServers.clear();
		ServerByID.clear();

		RemoveAllChildren();

		return S_SYSTEM_OK;
	}

	
	
	//////////////////////////////////////////////////////////////////////////////
	//
	//	Configuration data Access point
	//


	// Load Config file
	HRESULT LoadConfig( const char* strConfigFileName )
	{
		ServerConfigParser parser ( __ConfigData );

		int result = xmlSAXUserParseFile( parser, &parser, strConfigFileName);

		if (result != 0)
		{
 			// error log
			return E_SYSTEM_FAIL;
		}
		xmlCleanupParser();
		return S_SYSTEM_OK;
	}

	// Clear Config data
	HRESULT ClearConfig()
	{
		return __ConfigData.ClearConfig();
	}

	// Get Config data instance
	const ConfigData& GetConfig()
	{
		return __ConfigData;
	}



}; // namespace Config
}; // namespace Svr
}; // namespace BR


