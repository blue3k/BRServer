////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : Generated
// 
// Description : CharacterDataServer Message parser definitions
// 
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol/SFProtocol.h"
#include "Net/SFMessage.h"
#include "Types/SFEngineTypedefs.h"
#include "Container/SFArray.h"
#include "Protocol/SvrProtocol.h"



namespace SF
{
 	namespace Message
	{
 		namespace CharacterDataServer
		{
 			// Cmd: Add character data to the account
			class AddCharacterDataCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				const char* m_CharacterName;
				VariableTable m_Attributes;
			public:
				AddCharacterDataCmd()
				:m_CharacterName(nullptr)
					{}

				AddCharacterDataCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
				,m_CharacterName(nullptr)
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const char* GetCharacterName() const	{ return m_CharacterName; };
				const VariableTable& GetAttributes() const	{ return m_Attributes; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const char* InCharacterName, const VariableTable &InAttributes );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AddCharacterDataCmd : public MessageBase

			class AddCharacterDataRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
				uint64_t m_CharacterUID;
			public:
				AddCharacterDataRes()
					{}

				AddCharacterDataRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const uint64_t& GetCharacterUID() const	{ return m_CharacterUID; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const uint64_t &InCharacterUID );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AddCharacterDataRes : public MessageBase

			// Cmd: Remove character data
			class RemoveCharacterDataCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
			public:
				RemoveCharacterDataCmd()
					{}

				RemoveCharacterDataCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RemoveCharacterDataCmd : public MessageBase

			class RemoveCharacterDataRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				RemoveCharacterDataRes()
					{}

				RemoveCharacterDataRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RemoveCharacterDataRes : public MessageBase

			// Cmd: Get character list
			class GetCharacterDataListCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
			public:
				GetCharacterDataListCmd()
					{}

				GetCharacterDataListCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetCharacterDataListCmd : public MessageBase

			class GetCharacterDataListRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
				StringCrc32 m_CharacterName;
				VariableTable m_Attributes;
			public:
				GetCharacterDataListRes()
					{}

				GetCharacterDataListRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const VariableTable& GetAttributes() const	{ return m_Attributes; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetCharacterDataListRes : public MessageBase

			// Cmd: Get character data
			class GetCharacterDataCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
			public:
				GetCharacterDataCmd()
					{}

				GetCharacterDataCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetCharacterDataCmd : public MessageBase

			class GetCharacterDataRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
				StringCrc32 m_CharacterName;
				VariableTable m_Attributes;
			public:
				GetCharacterDataRes()
					{}

				GetCharacterDataRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const VariableTable& GetAttributes() const	{ return m_Attributes; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class GetCharacterDataRes : public MessageBase

			// Cmd: Set(add or update) attribute value
			class SetAttributeCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
				VariableTable m_Attributes;
			public:
				SetAttributeCmd()
					{}

				SetAttributeCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const VariableTable& GetAttributes() const	{ return m_Attributes; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const VariableTable &InAttributes );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class SetAttributeCmd : public MessageBase

			class SetAttributeRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				SetAttributeRes()
					{}

				SetAttributeRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class SetAttributeRes : public MessageBase

			// Cmd: Remove an attribute value
			class RemoveAttributesCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
				ArrayView<StringCrc32> m_AttributeNames;
			public:
				RemoveAttributesCmd()
					{}

				RemoveAttributesCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const Array<StringCrc32>& GetAttributeNames() const	{ return m_AttributeNames; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const Array<StringCrc32>& InAttributeNames );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RemoveAttributesCmd : public MessageBase

			class RemoveAttributesRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				RemoveAttributesRes()
					{}

				RemoveAttributesRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class RemoveAttributesRes : public MessageBase

			// Cmd: Attribute add
			class AttributeValueAddCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
				StringCrc32 m_AttributeName;
				float m_Value;
			public:
				AttributeValueAddCmd()
					{}

				AttributeValueAddCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const StringCrc32& GetAttributeName() const	{ return m_AttributeName; };
				const float& GetValue() const	{ return m_Value; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueAddCmd : public MessageBase

			class AttributeValueAddRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				AttributeValueAddRes()
					{}

				AttributeValueAddRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueAddRes : public MessageBase

			// Cmd: Attribute subtract
			class AttributeValueSubCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
				StringCrc32 m_AttributeName;
				float m_Value;
			public:
				AttributeValueSubCmd()
					{}

				AttributeValueSubCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const StringCrc32& GetAttributeName() const	{ return m_AttributeName; };
				const float& GetValue() const	{ return m_Value; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const float &InValue );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueSubCmd : public MessageBase

			class AttributeValueSubRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				AttributeValueSubRes()
					{}

				AttributeValueSubRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueSubRes : public MessageBase

			// Cmd: Compare and exchange attribute value
			class AttributeValueCASCmd : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 1,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 1,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				uint16_t m_RouteHopCount;
				PlayerID m_PlayerID;
				StringCrc32 m_CharacterName;
				StringCrc32 m_AttributeName;
				StringCrc32 m_AttributeType;
				uint64_t m_Expected;
				uint64_t m_NewValue;
			public:
				AttributeValueCASCmd()
					{}

				AttributeValueCASCmd( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const uint16_t& GetRouteHopCount() const	{ return m_RouteHopCount; };
				const PlayerID& GetPlayerID() const	{ return m_PlayerID; };
				const StringCrc32& GetCharacterName() const	{ return m_CharacterName; };
				const StringCrc32& GetAttributeName() const	{ return m_AttributeName; };
				const StringCrc32& GetAttributeType() const	{ return m_AttributeType; };
				const uint64_t& GetExpected() const	{ return m_Expected; };
				const uint64_t& GetNewValue() const	{ return m_NewValue; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const uint16_t &InRouteHopCount, const PlayerID &InPlayerID, const StringCrc32 &InCharacterName, const StringCrc32 &InAttributeName, const StringCrc32 &InAttributeType, const uint64_t &InExpected, const uint64_t &InNewValue );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueCASCmd : public MessageBase

			class AttributeValueCASRes : public MessageBase
			{
 			public:
				static const MessageID MID;
				// Parameter type informations for template
				enum ParameterTypeInfo
				{
 					HasPlayerID = 0,
					HasTransactionID = 1,
					HasRouteContext = 1,
					HasRouteHopCount = 0,
					HasSender = 0,
				}; // enum ParameterTypeInfo
			public:
				uint64_t GetPlayerID() { return 0; }
				uint32_t GetRouteHopCount() { return 0; }
				uint64_t GetSender() { return 0; }
			private:
				RouteContext m_RouteContext;
				uint64_t m_TransactionID;
				Result m_Result;
			public:
				AttributeValueCASRes()
					{}

				AttributeValueCASRes( MessageDataPtr &&pMsg )
					: MessageBase(std::forward<MessageDataPtr>(pMsg))
					{}

					MessageUsage GetMessageUsage() { return MessageUsage_None; }

				const RouteContext& GetRouteContext() const	{ return m_RouteContext; };
				const uint64_t& GetTransactionID() const	{ return m_TransactionID; };
				const Result& GetResult() const	{ return m_Result; };

				static Result TraceOut(const char* prefix, const MessageDataPtr& pMsg);

				virtual Result ParseMessage( MessageData* pIMsg );
				static Result ParseMessageToMessageBase( IHeap& memHeap, MessageDataPtr&& pIMsg, MessageBase* &pMsgBase );

				static MessageData* Create( IHeap& memHeap, const RouteContext &InRouteContext, const uint64_t &InTransactionID, const Result &InResult );

				Result OverrideRouteContextDestination( EntityUID to );

				Result OverrideRouteInformation( EntityUID to, unsigned hopCount );

			}; // class AttributeValueCASRes : public MessageBase





		}; // namespace CharacterDataServer
	}; // namespace Message
}; // namespace SF


