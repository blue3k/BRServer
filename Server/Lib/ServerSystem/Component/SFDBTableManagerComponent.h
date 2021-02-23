////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) StormForge
// 
// Author : KyungKun Ko
//
// Description : Server message endpoint
//	
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SFTypedefs.h"
#include "Types/SFEngineTypedefs.h"
#include "Util/SFStringCrc32.h"
#include "Util/SFString.h"
#include "Object/SFLibraryComponent.h"
#include "Service/DataTableManagerService.h"
#include "DB/DataSource.h"
#include "DB/DBConfig.h"
#include "DB/Session.h"



namespace SF
{
	
	class DBTableManagerComponent : public LibraryComponent, public DataTableManagerService
	{
	public:

		static constexpr StringCrc64 TypeName = "DBTableManagerComponent";

	private:

		IHeap& m_Heap;

		DB::DataSource* m_pDataSource{};

	public:

		// Constructor
		DBTableManagerComponent();
		~DBTableManagerComponent();

		// Heap
		IHeap& GetHeap() { return m_Heap; }

		virtual const StringCrc64& GetTypeName() override { return TypeName; }

		virtual Result LoadDataTable(const char* tableName, StringCrc32 keyColumnName, const char* tableSrcPath) override;

		// Initialize component
		virtual Result InitializeComponent() override;
		// Terminate component
		virtual void DeinitializeComponent() override;
	};
}

