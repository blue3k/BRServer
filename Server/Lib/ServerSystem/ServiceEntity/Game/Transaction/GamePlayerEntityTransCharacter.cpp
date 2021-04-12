////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) The Braves
// 
// Author : KyungKun Ko
//
// Description : game player entity implementation
//
////////////////////////////////////////////////////////////////////////////////


#include "ServerSystemPCH.h"

#include "ResultCode/SFResultCodeLibrary.h"
#include "ResultCode/SFResultCodeGame.h"
#include "ResultCode/SFResultCodeLogin.h"
#include "ResultCode/SFResultCodeEngine.h"
#include "MemoryManager/SFMemoryPool.h"
#include "Types/SFEngineTypedefs.h"
#include "GameConst.h"

#include "Server/BrServerUtil.h"
#include "SvrTrace.h"

#include "GamePlayerEntityTrans.h"
#include "ServiceEntity/Game/GamePlayerEntity.h"
#include "ServiceEntity/Game/Transaction/GamePlayerEntityTransCharacter.h"

#include "Server/BrServer.h"

#include "Game/GameDB.h"
#include "Game/GameQuery.h"



namespace SF {
	namespace Svr {


		/////////////////////////////////////////////////////////////////////////////////////////////////
		//

		PlayerTransCreateCharacter::PlayerTransCreateCharacter(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
		{
			AddSubAction<DB::QueryCreateCharacterCmd>(&PlayerTransCreateCharacter::RequestCreateCharacterDB, &PlayerTransCreateCharacter::OnCreateCharacterRes);
			AddSubAction(&PlayerTransCreateCharacter::FinalizeSuccess);
		}

		Result PlayerTransCreateCharacter::RequestCreateCharacterDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			auto visualData = GetVisualData();
			auto requestAttributs = GetAttributes();

			visualData.SetValue("level", 1);

			requestAttributs.SetValue("exp", 0);

			svrCheck(Svr::GetServerComponent<DB::GameDB>()->CreateCharacter(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetCharacterName(), visualData, requestAttributs));

			return hr;
		}

		Result PlayerTransCreateCharacter::OnCreateCharacterRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QueryCreateCharacterCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				m_CharacterId = pDBRes->CharacterId;
			}
			else
			{
				CloseTransaction(ResultCode::DB_FAILURE);
			}

			return hr;
		}

		// Start Transaction
		Result PlayerTransCreateCharacter::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAuthTicket() == 0)
			{
				svrErrorClose(ResultCode::INVALID_TICKET);
			}

			if (StrUtil::IsNullOrEmpty(GetCharacterName()))
			{
				svrErrorClose(ResultCode::INVALID_ARG);
			}

			svrCheck(super::StartTransaction());

			return hr;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////
		//

		PlayerTransDeleteCharacter::PlayerTransDeleteCharacter(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
		{
			AddSubAction<DB::QueryDeleteCharacterCmd>(&PlayerTransDeleteCharacter::RequestDeleteCharacterDB, &PlayerTransDeleteCharacter::OnDeleteCharacterRes);
			AddSubAction(&PlayerTransDeleteCharacter::FinalizeSuccess);
		}

		Result PlayerTransDeleteCharacter::RequestDeleteCharacterDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			svrCheck(Svr::GetServerComponent<DB::GameDB>()->DeleteCharacter(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetCharacterID()));

			return hr;
		}

		Result PlayerTransDeleteCharacter::OnDeleteCharacterRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QueryGetCharacterCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				CloseTransaction(ResultCode::SUCCESS);
			}
			else
			{
				CloseTransaction(ResultCode::DB_FAILURE);
			}

			return hr;
		}

		// Start Transaction
		Result PlayerTransDeleteCharacter::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAuthTicket() == 0)
			{
				svrErrorClose(ResultCode::INVALID_TICKET);
			}

			if (GetCharacterID() != 0)
			{
				svrErrorClose(ResultCode::INVALID_ARG);
			}

			svrCheck(super::StartTransaction());

			return hr;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////
		//

		PlayerTransGetCharacter::PlayerTransGetCharacter(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
		{
			AddSubAction<DB::QueryGetCharacterCmd>(&PlayerTransGetCharacter::RequestGetCharacterDB, &PlayerTransGetCharacter::OnGetCharacterRes);
			AddSubAction(&PlayerTransGetCharacter::FinalizeSuccess);
		}

		Result PlayerTransGetCharacter::RequestGetCharacterDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetCharacter(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetCharacterID()));

			return hr;
		}

		Result PlayerTransGetCharacter::OnGetCharacterRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QueryGetCharacterCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				if (pDBRes->RowsetResults.size() > 0)
				{
					m_CharacterData = pDBRes->RowsetResults[0];
					CloseTransaction(ResultCode::SUCCESS);
				}
				else
				{
					CloseTransaction(ResultCode::NO_DATA_EXIST);
				}
			}
			else
			{
				CloseTransaction(ResultCode::DB_FAILURE);
			}

			return hr;
		}

		// Start Transaction
		Result PlayerTransGetCharacter::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAuthTicket() == 0)
			{
				svrErrorClose(ResultCode::INVALID_TICKET);
			}

			if (GetCharacterID() == 0)
			{
				svrErrorClose(ResultCode::INVALID_ARG);
			}

			svrCheck(super::StartTransaction());

			return hr;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////
		//

		PlayerTransGetCharacterList::PlayerTransGetCharacterList(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
		{
			AddSubAction<DB::QueryGetCharacterListCmd>(&PlayerTransGetCharacterList::RequestGetCharacterListDB, &PlayerTransGetCharacterList::OnGetCharacterListRes);
			AddSubAction(&PlayerTransGetCharacterList::FinalizeSuccess);
		}

		Result PlayerTransGetCharacterList::RequestGetCharacterListDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetCharacterList(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID()));

			return hr;
		}

		Result PlayerTransGetCharacterList::OnGetCharacterListRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QueryGetCharacterListCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				if (pDBRes->RowsetResults.size() > 0)
				{
					m_Characters.Clear();
					m_Characters.resize(pDBRes->RowsetResults.size());
					int iChar = 0;
					for (auto& itRes : pDBRes->RowsetResults)
					{
						auto& characterData = m_Characters[iChar];
						auto* pVariable = itRes.GetVariable("CharacterId");
						if (pVariable != nullptr)
							characterData.SetVariable("CharacterId", *pVariable);

						pVariable = itRes.GetVariable("Name");
						if (pVariable != nullptr)
							characterData.SetVariable("Name", *pVariable);

						pVariable = itRes.GetVariable("VisualData");
						if (pVariable != nullptr)
							characterData.SetVariable("VisualData", *pVariable);

						iChar++;
					}
					CloseTransaction(ResultCode::SUCCESS);
				}
				else
				{
					CloseTransaction(ResultCode::NO_DATA_EXIST);
				}
			}
			else
			{
				CloseTransaction(ResultCode::DB_FAILURE);
			}

			return hr;
		}

		// Start Transaction
		Result PlayerTransGetCharacterList::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAuthTicket() == 0)
			{
				svrErrorClose(ResultCode::INVALID_TICKET);
			}

			svrCheck(super::StartTransaction());

			return hr;
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////
		//

		PlayerTransSelectCharacter::PlayerTransSelectCharacter(IHeap& heap, const MessageDataPtr& pIMsg)
			: MessageTransaction(heap, pIMsg)
		{
			AddSubAction<DB::QuerySaveCharacterCmd>(&PlayerTransSelectCharacter::RequestSaveCharacterDB, &PlayerTransSelectCharacter::OnSaveCharacterRes);
			AddSubAction<DB::QueryGetCharacterCmd>(&PlayerTransSelectCharacter::RequestGetCharacterDB, &PlayerTransSelectCharacter::OnGetCharacterRes);
			AddSubAction(&PlayerTransSelectCharacter::FinalizeSuccess);
		}

		Result PlayerTransSelectCharacter::RequestSaveCharacterDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetCharacterID() != 0)
			{
				svrCheck(Svr::GetServerComponent<DB::GameDB>()->SaveCharacter(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetMyOwner()->GetCharacterID(), GetMyOwner()->GetCharacterVisualData(), GetMyOwner()->GetCharacterData()));
			}
			else
			{
				hr = ResultCode::SUCCESS_FALSE;
			}

			return hr;
		}

		Result PlayerTransSelectCharacter::OnSaveCharacterRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QuerySaveCharacterCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				GetMyOwner()->ClearCharacterData();

				if (pDBRes->RowsetResults.size() > 0)
				{
				}
				else
				{
					CloseTransaction(ResultCode::ENGINE_INVALID_CHARACTER);
				}
			}
			else
			{
				CloseTransaction(ResultCode::DB_FAILURE);
			}

			return hr;
		}

		Result PlayerTransSelectCharacter::RequestGetCharacterDB()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetCharacterID() == 0)
			{
				GetMyOwner()->ClearCharacterData();
				return hr = ResultCode::SUCCESS_FALSE;
			}

			svrCheck(Svr::GetServerComponent<DB::GameDB>()->GetCharacter(GetTransID(), GetMyOwner()->GetShardID(), GetMyOwner()->GetPlayerID(), GetCharacterID()));

			return hr;
		}

		Result PlayerTransSelectCharacter::OnGetCharacterRes(Svr::TransactionResult* pRes)
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});
			auto* pDBRes = pRes->GetResultData<DB::QueryGetCharacterCmd>();

			svrCheck(pRes->GetResult());

			// succeeded to login
			if (pDBRes->Result == 0)
			{
				if (pDBRes->RowsetResults.size() > 0)
				{
					m_CharacterDataAll = pDBRes->RowsetResults[0];

					auto& visualData = GetMyOwner()->GetCharacterVisualData();
					visualData.Clear();
					visualData.FromBinData(m_CharacterDataAll.GetValueBLOB("VisualData"));

					auto& charData = GetMyOwner()->GetCharacterData();
					charData.Clear();
					charData.FromBinData(m_CharacterDataAll.GetValueBLOB("BinData"));

					GetMyOwner()->SetCharacterID(GetCharacterID());

					GetMyOwner()->OnCharacterDataLoaded();

					CloseTransaction(ResultCode::SUCCESS);
				}
				else
				{
					CloseTransaction(ResultCode::NO_DATA_EXIST);
				}
			}
			else
			{
				CloseTransaction(ResultCode::LOGIN_INVALID_SIGNATURE);
			}

			return hr;
		}

		// Start Transaction
		Result PlayerTransSelectCharacter::StartTransaction()
		{
			ScopeContext hr([this](Result hr)
				{
					if (!hr) CloseTransaction(hr);
				});

			if (GetMyOwner()->GetAccountID() == 0 || GetMyOwner()->GetAuthTicket() == 0)
			{
				svrErrorClose(ResultCode::INVALID_TICKET);
			}

			svrCheck(super::StartTransaction());

			return hr;
		}


	} // namespace Svr
} // namespace SF 

