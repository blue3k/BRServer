////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Game Vote
//	
////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SFTypedefs.h"
#include "Types/BrGameTypes.h"
#include "Memory/MemoryPool.h"
#include "Common/ClassUtil.h"
#include "Common/ArrayUtil.h"
#include "Common/GameConst.h"

namespace BR {
namespace ConspiracyGameInstanceServer {

	class GameInstanceEntity;
	class GamePlayer;
	class GamePlaySystem;


	//////////////////////////////////////////////////////////////////////////
	//
	//	GameState class
	//

	class GameVote
	{
		enum class VoteState
		{
			None,
			Voting,				// Normal state, waiting players vote
			InHurry,			// Left players and long time sleeper will be voted forcely
			ForceVote,			// Any one who haven't voted will be voted forcely
		};

	private:

		GameInstanceEntity* m_Owner;

		// Vote Status
		BRCLASS_ATTRIBUTE(VoteState,VoteState);

	public:
		GameVote(GameInstanceEntity* Owner);
		virtual ~GameVote() {}

		GamePlaySystem& GetGamePlaySystem();
		FORCEINLINE GameInstanceEntity& GetOwner()			{ return *m_Owner; }

		// Iniciate vote
		virtual Result IniciateVote();

		// vote 
		virtual Result Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget );

		// check weather the vote is ended or not
		virtual bool IsVoteEnd() = 0;

		// Make all AI(left players and sleepers) be voted
		virtual void ForceAIVoted() = 0;

		// Make all player be voted
		virtual void ForceAllVoted() = 0;

		// Update vote state
		virtual void UpdateVote(DurationMS ulTimeInState);

	protected:
		//////////////////////////////////////

		void VoteRandomTarget( GamePlayer* pVoter, Array<GamePlayer*>& voteTargets );
	};

	

	class GameVoteSuspect : public GameVote
	{
	private:
		bool m_IsInVoting;

		// suspects list
		PlayerID m_VoteRankers[2];
		UINT m_VoteRates[2];

		// Gather vote targets
		// returns count of members who need to be voted
		UINT GatherVoteTarget(Array<GamePlayer*>& voteTargets);

	public:
		GameVoteSuspect(GameInstanceEntity* Owner)
			:GameVote(Owner), m_IsInVoting(false)
		{}
		virtual ~GameVoteSuspect() {}

		// set suspect with rate
		PlayerID GetVoteRanker( int index );
		void SetVoteRanker( PlayerID playerID, UINT rate );

		// Iniciate vote
		virtual Result IniciateVote() override;

		virtual Result Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget ) override;
		
		// Make all AI(left players and sleepers) be voted
		virtual void ForceAIVoted() override;

		// Make all player be voted
		virtual void ForceAllVoted() override;

		virtual bool IsVoteEnd() override;
	};


	class GameVoteHanging : public GameVote
	{
	private:
		bool m_IsInVoting;

		PlayerID m_PlayerToHang;
		UINT m_TopRate;

		UINT GatherVoteTarget(Array<GamePlayer*>& voteTargets);

	public:
		GameVoteHanging(GameInstanceEntity* Owner)
			:GameVote(Owner),m_IsInVoting(false), m_PlayerToHang(0), m_TopRate(0)
		{}
		virtual ~GameVoteHanging() {}


		PlayerID GetPlayerToHang();

		// Iniciate vote
		virtual Result IniciateVote() override;

		virtual Result Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget ) override;
		
		// Make all AI(left players and sleepers) be voted
		virtual void ForceAIVoted() override;

		// Make all player be voted
		virtual void ForceAllVoted() override;

		virtual bool IsVoteEnd() override;
	};


	// Night vote
	class GameVoteNight : public GameVote
	{
	public:
		enum VoteFlags {
			BODYGUARD		= 0x1,
			OWLMAN			= 0x2,
			FREEMASON		= 0x4,
			MEDIUM			= 0x8,
		};

		UINT m_VotingFlags;

		UINT GatherVoteTarget(Array<GamePlayer*>& voteTargets);
		UINT GatherVoteTargetForHunting(Array<GamePlayer*>& voteTargets);
		UINT GatherVoteTargetForSeer(Array<GamePlayer*>& voteTargets);

	private:

		BRCLASS_ATTRIBUTE(PlayerID,SeersChoice);
		BRCLASS_ATTRIBUTE(PlayerID,BodyGuardsChoice);

		BRCLASS_ATTRIBUTE_READONLY(PlayerID,PlayerToKill);
		UINT m_TopRate;

		bool m_IsInVoting;


	public:
		GameVoteNight(GameInstanceEntity* Owner,UINT votingFlags);
		virtual ~GameVoteNight() {}

		FORCEINLINE bool IsFlagSet( VoteFlags flag )	{ return (m_VotingFlags&flag) != 0; }

		// Iniciate vote
		virtual Result IniciateVote() override;

		virtual Result Vote( GamePlayer* pVoter, GamePlayer *pVoteTarget ) override;
		
		// Make all AI(left players and sleepers) be voted
		virtual void ForceAIVoted() override;

		// Make all player be voted
		virtual void ForceAllVoted() override;

		virtual bool IsVoteEnd() override;
	
	};


}; // namespace ConspiracyGameInstanceServer
}; // namespace BR







