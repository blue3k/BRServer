////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2017 StormForge
// 
// Author : KyungKun Ko
//
// Description : Server Config
//	
//
////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "SFEnum.h"
#include "String/FixedString32.h"

#include <libxml/parser.h>
#include <libxml/tree.h>



namespace SF
{

	const char* Enum<GameID>::GetName() const
	{
		return "GameID";
	}

	static constexpr const char* __GameID_NameList[] =
	{
		"None",
		"Conspiracy",
		"MyTownHero",
	};

	static constexpr FixedString __GameID_NameCrcList[] =
	{
		"None",
		"Conspiracy",
		"MyTownHero",
	};


	const char* Enum<GameID>::GetValueName(GameID value) const
	{

		return __GameID_NameList[(int)value];
	}


	FixedString Enum<GameID>::GetValueNameCrc(GameID value) const
	{
		return __GameID_NameCrcList[(int)value];
	}

	GameID Enum<GameID>::GetValue(const char* valueName) const
	{
		FixedString valueNameCrc = valueName;
		return GetValue(valueNameCrc);
	}

	GameID Enum<GameID>::GetValue(FixedString valueName) const
	{
		FixedString valueNameCrc = valueName;
		for (int iEnum = 0; iEnum < countof(__GameID_NameList); iEnum++)
		{
			if (__GameID_NameCrcList[iEnum] == valueNameCrc)
				return (GameID)iEnum;
		}

		return (GameID)(0);
	}

	template<> class Enum<GameID>;






	const char* Enum<DBClusterType>::GetName() const
	{
		return "DBClusterType";
	}

	static constexpr const char* __DBClusterType_NameList[] =
	{
		"Normal",
		"Sharding",
	};

	static constexpr FixedString __DBClusterType_NameCrcList[] =
	{
		"Normal",
		"Sharding",
	};


	const char* Enum<DBClusterType>::GetValueName(DBClusterType value) const
	{

		return __DBClusterType_NameList[(int)value];
	}


	FixedString Enum<DBClusterType>::GetValueNameCrc(DBClusterType value) const
	{
		return __DBClusterType_NameCrcList[(int)value];
	}

	DBClusterType Enum<DBClusterType>::GetValue(const char* valueName) const
	{
		FixedString valueNameCrc = valueName;
		return GetValue(valueNameCrc);
	}

	DBClusterType Enum<DBClusterType>::GetValue(FixedString valueName) const
	{
		FixedString valueNameCrc = valueName;
		for (int iEnum = 0; iEnum < countof(__GameID_NameList); iEnum++)
		{
			if (__DBClusterType_NameCrcList[iEnum] == valueNameCrc)
				return (DBClusterType)iEnum;
		}

		return (DBClusterType)(0);
	}

	template<> class Enum<DBClusterType>;
}


