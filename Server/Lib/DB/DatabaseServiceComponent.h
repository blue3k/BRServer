////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves 
// 
// Author: Kyungkun Ko
//
// Description : Database interface
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "SFAssert.h"
#include "Object/SFLibraryComponent.h"


namespace SF
{

	//////////////////////////////////////////////////////////////////////////////////
	//
	//	DB Factory Class 
	//

	class DatabaseServiceComponent : public LibraryComponent
	{
	public:

		static constexpr StringCrc64 TypeName = "DatabaseServiceComponent";

	private:

		SFUniquePtr<DatabaseService> m_DBService;

	public:

		DatabaseServiceComponent();
		virtual ~DatabaseServiceComponent();

		virtual const StringCrc64& GetTypeName() override { return TypeName; };

		virtual Result InitializeComponent() override;
		virtual void DeinitializeComponent() override;
	};

} // namespace SF
