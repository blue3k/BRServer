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

#pragma once

#include "String/FixedString32.h"
#include "String/SFString.h"
#include "Container/CircularPageQueue.h"
#include "Thread/Task.h"

#include "ZooKeeper/SFZooKeeper.h"


typedef struct _xmlNode xmlNode;


namespace SF
{
	enum class DBClusterType
	{
		Normal,
		Sharding,
	};


	template<class EnumType>
	class Enum
	{
	public:
		typedef const char* constCharPtr;

	public:

		constexpr Enum() {}

		const char* GetName() const { return nullptr; }
		const char* GetValueName(EnumType value) const { return nullptr; }
		FixedString GetValueNameCrc(EnumType value) const { return nullptr; }
		EnumType GetValue(const char*) const;
		EnumType GetValue(FixedString valueName) const;
	};



	template<>
	class Enum<GameID>
	{
	public:

		constexpr Enum() {}

		const char* GetName() const;

		const char* GetValueName(GameID value) const;

		FixedString GetValueNameCrc(GameID value) const;

		GameID GetValue(const char*) const;

		GameID GetValue(FixedString valueName) const;
	};



	extern template class Enum<GameID>;



	template<>
	class Enum<DBClusterType>
	{
	public:

		constexpr Enum() {}

		const char* GetName() const;

		const char* GetValueName(DBClusterType value) const;

		FixedString GetValueNameCrc(DBClusterType value) const;

		DBClusterType GetValue(const char*) const;

		DBClusterType GetValue(FixedString valueName) const;
	};



	extern template class Enum<DBClusterType>;

}

