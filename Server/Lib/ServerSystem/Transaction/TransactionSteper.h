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

	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TransactionSteper class
	//

	class TransactionStep
	{
	private:
		// Step tick, Count of ProcessStep called
		uint m_uiStepTick;

	public:
		TransactionStep() : m_uiStepTick(0)			{}
		virtual ~TransactionStep()	{}

		// Get step tick
		inline uint GetStepTick()			{ return m_uiStepTick; }
		// increase step tick
		inline void NextStep()				{ m_uiStepTick++; }

		// Process step
		virtual Result ProcessStep( Svr::TransactionResult* &pRes, bool &IsCompleted ) = 0;
	};


	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TransactionSteper class
	//

	template< class OwnerEntityType >
	class TransactionStepT : public TransactionStep
	{
	private:
		Svr::Transaction *m_pOwnerTransaction;

	public:
		TransactionStepT( Svr::Transaction *pOwnerTrans )
			:m_pOwnerTransaction(pOwnerTrans)
		{
		}

		const TransactionID& GetTransID()	{ return m_pOwnerTransaction->GetTransID(); }
		OwnerEntityType* GetMyOwner()		{ return (OwnerEntityType*)m_pOwnerTransaction->GetOwnerEntity(); }
	};



	////////////////////////////////////////////////////////////////////////////////////////
	//
	//	TransactionSteper class
	//

	template< int iMaxStep >
	class TransactionSteper
	{
	private:
		// Current step
		uint m_CurStep;

		// Transaction steps
		TransactionStep*	m_Steps[iMaxStep];


	public:
		// Constructor
		TransactionSteper()
			:m_CurStep(0)
		{
			memset( m_Steps, 0, sizeof(m_Steps) );
		}

		virtual ~TransactionSteper() {}

		bool IsCompletedAllStep()
		{
			return m_CurStep >= iMaxStep;
		}

		// Assign step
		Result AssignStep( uint iStep, TransactionStep *pStep )
		{
			if( iStep >= iMaxStep )
			{
				AssertRel( 0 );
				return ResultCode::INVALID_ARG;
			}

			m_Steps[iStep] = pStep;
			return ResultCode::SUCCESS;
		}

		// Process step
		Result ProcessStep( Svr::TransactionResult* &pRes )
		{
			bool IsCompleted = true;
			while( !IsCompletedAllStep() && IsCompleted )
			{
				if( m_Steps[m_CurStep] )
				{
					IsCompleted = false;
					Result hr = m_Steps[m_CurStep]->ProcessStep( pRes, IsCompleted );

					if( !(hr) )
					{
						svrTrace( Error, "Error, FailedStep {0}:{1}", m_CurStep, typeid(*m_Steps[m_CurStep]).name() );
						m_CurStep++;
						pRes = nullptr;
					}
					else
					{
						m_Steps[m_CurStep]->NextStep();
						if( IsCompleted ) // If current step completed then move to next step
						{
							m_CurStep++;
							pRes = nullptr;
						}
					}
				}
				else
				{
					m_CurStep++;
					IsCompleted  = true;
				}

				if (pRes != nullptr)
				{
					delete pRes;
					pRes = nullptr;
				}
			}

			return ResultCode::SUCCESS;
		}
	};



}; // namespace Svr
}; // namespace SF

