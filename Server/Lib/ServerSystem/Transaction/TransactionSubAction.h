////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Transaction stepper
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "ResultCode/SFResultCodeLibrary.h"
#include "Net/SFMessage.h"




namespace SF {
	namespace Svr {

		class TransactionResult;
		class Transaction;

		////////////////////////////////////////////////////////////////////////////////////////
		//
		//	TransactionSubAction class
		//

		class TransactionSubAction
		{
		public:
			TransactionSubAction() {}
			virtual ~TransactionSubAction() {}

			virtual void RegisterResultHandlers() {}

			virtual Result Process() = 0;
		};


		////////////////////////////////////////////////////////////////////////////////////////
		//
		//	TransactionSubActionManager class
		//

		template< int iMaxStep = 6 >
		class TransactionSubActionManager
		{
		private:
			// Current action step
			uint m_CurStep = 0;

			// Transaction steps
			StaticArray<TransactionSubAction*, iMaxStep> m_Actions;


		public:
			// Constructor
			TransactionSubActionManager(IHeap& heap)
				: m_Actions(heap)
			{
			}

			virtual ~TransactionSubActionManager() {}

			bool IsCompleted()
			{
				return m_CurStep >= m_Actions.size();
			}

			// add action
			Result AddAction(TransactionSubAction* pStep)
			{
				return m_Actions.push_back(pStep);
			}

			Result AddActionFront(TransactionSubAction* pStep)
			{
				return m_Actions.insert(0, pStep);
			}

			// Process 
			Result Process()
			{
				Result hr;
				while (!IsCompleted())
				{
					auto curAction = m_Actions[m_CurStep];
					hr = curAction->Process();
					if (!hr)
					{
						svrTrace(Error, "Error, Failed transaction action, Step {0}", m_CurStep);
					}
					else if (hr == ResultCode::SUCCESS_YIELD)
					{
						// wait more result
					}
					else if (hr == ResultCode::SUCCESS_FALSE)
					{
						// This action did nothing, proceeded to next
						m_CurStep++;
						continue;
					}
					else
					{
						m_CurStep++;
					}

					break;
				}


				return hr;
			}
		};



	} // namespace Svr

} // namespace SF

