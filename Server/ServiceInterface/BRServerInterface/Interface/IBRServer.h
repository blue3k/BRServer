////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Blue3k
// 
// Author : KyungKun Ko
//
// Description : Server interface
//
////////////////////////////////////////////////////////////////////////////////

#include "ServerSystem/SvrTrace.h"
#include "ServerSystem/BrServer.h"



namespace BR {

	namespace Svr {

		class IBRServer
		{
		private:

		public:

			IBRServer();
			~IBRServer();

			HRESULT StartServer();
			HRESULT StopServer();
		};

	}
}
