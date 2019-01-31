////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description:   Simple finite state machine class
//
////////////////////////////////////////////////////////////////////////////////

using System.Collections;
using System.Collections.Generic;
using System;

namespace BR
{
	public class SimpleFSM
	{
		// State class for Simple FSM
		public abstract class State : IDisposable
		{
			// MyFSM
			public SimpleFSM  MyFSM {get; set;}

			// Allowed transition
			public HashSet<Type> AllowedTransition {get; protected set;}

			// Disallowed transition
			public HashSet<Type> DisallowedTransition {get; protected set;}

			public State()
			{
				AllowedTransition = new HashSet<Type>();
				DisallowedTransition = new HashSet<Type>();
			}

			// Add allowed transition
			public void AddAllowedTransition<T>() where T : State
			{
				Type curType = typeof(T);
				while (curType != null
				       && !(curType.Equals(typeof(State)))
				       && !AllowedTransition.Contains(curType))
				{
					AllowedTransition.Add(curType);
					curType = curType.BaseType;
				}
			}

			// Add Disallowed transition
			public void AddDisallowedTransition<T>() where T : State
			{
				Type curType = typeof(T);
				while (curType != null
				       && !(curType.Equals(typeof(State)))
				       && !DisallowedTransition.Contains(curType))
				{
					DisallowedTransition.Add(curType);
					curType = curType.BaseType;
				}
			}

			// Check whether stateType is allowed
			public bool IsAllowedTransition<T>() where T : State
			{
				return IsAllowedTransition(typeof(T));
			}

			public bool IsAllowedTransition(Type stateType)
			{
				return (AllowedTransition.Count == 0
				        || AllowedTransition.Contains(stateType))
					&& (DisallowedTransition.Count == 0
					    || !DisallowedTransition.Contains(stateType));
			}


			// Transit to next state
			public void SetNextState(Type state)
			{
				MyFSM.SetNextState(state);
			}

			// Transit to next state
			public void SetNextState<NextState>() where NextState : State
			{
				MyFSM.SetNextState(typeof(NextState));
			}


			// Initialize state when it created
			public virtual void Awake()
			{
			}

			// Initialize state to use
			public virtual void Start()
			{
			}

			// clear all
			public virtual void Dispose()
			{
			}

			public abstract void OnEnter();
			public abstract void OnLeave();
			public abstract void UpdateState();
		}


		// state map
		private Dictionary<Type, State> m_stateByType = new Dictionary<Type, State>();


		// Current state
		private State m_CurState = null;

		// Reserved state to change
		private State m_NextState = null;

		// Previous state
		private State m_PrevState = null;


		// Create and registering states
		public StateType CreateState<StateType>() where StateType : State
		{
			StateType state = GetState<StateType>();
			if (state == null)
			{
				state = Activator.CreateInstance<StateType>();
				state.MyFSM = this;
				state.Awake();
			}

			RegisterState((State)(object)state);

			return state;
		}


		// registering states
		public void RegisterState(State state)
		{
			if (state == null)
				throw new ArgumentException();

			state.MyFSM = this;
			Type curType = state.GetType();
			while ( curType !=null 
			       && !(curType.Equals(typeof(State))) 
			       && !m_stateByType.ContainsKey(curType) )
			{
				m_stateByType.Add(curType, state);
				curType = curType.BaseType;
			}

			state.Start();
		}

		// check whether is allowed transition of not
		public bool IsAllowedTransition<StateType>() where StateType : State
		{
			return IsAllowedTransition(typeof(StateType));
		}

		public bool IsAllowedTransition(Type StateType)
		{
			return (m_NextState == null || m_NextState.IsAllowedTransition(StateType))
				&& (m_CurState == null || m_CurState.IsAllowedTransition(StateType));
		}

		public State GetNextState()
		{
			return m_NextState;
		}

		public StateType GetNextState<StateType>() where StateType : State
		{
			return m_NextState as StateType;
		}

		// reserve next state
		public void SetNextState<StateType>() where StateType : State
		{
			SetNextState(typeof(StateType));
		}

		public void SetNextState(Type nextState)
		{
			// 같은 프레임안에서 두번의 상태 변화가 발생!
			if (nextState != null && m_NextState != null)
				BRDebug.Warning("Next state Changed from " + m_NextState.GetType().Name + " to " + nextState.Name);

			State next = m_stateByType[nextState];

			if( !IsAllowedTransition(nextState) )
			{
				BRDebug.Log("This state Transition isn't allowed cur:" + m_CurState != null ? m_CurState.ToString() : "null" + 
				          ", nextPending:" + m_NextState != null ? m_NextState.GetType().ToString() : "null" +
				          "to " + nextState.ToString());
				return;
			}
			m_NextState = next;
		}


		public void ReEntrant()
		{
			m_NextState = m_CurState;
		}

		// query previous state
		public State GetPrevState()
		{
			return m_PrevState;
		}

		public Type GetPrevStateType()
		{
			if( m_PrevState != null )
				return m_PrevState.GetType();
			return default(Type);
		}

		// query current state
		public State GetCurState()
		{
			return m_CurState;
		}

		public StateType GetCurState<StateType>() where StateType : State
		{
			if( m_CurState != null )
				return (StateType)((object)m_CurState);
			//State state;
			//if (stateByType.TryGetValue(curStateId, out state)
			//    && state is StateType)
			//    return (StateType)((object)state);

			return default(StateType);
		}

		// query state by type
		public StateType GetState<StateType>() where StateType : State
		{
			State state;

			// if it's in map
			if (m_stateByType.TryGetValue(typeof(StateType), out state))
				return (StateType)((object)state);

			return default(StateType);
		}

		public void UpdateFSM()
		{
			// if not states are regerstered, we don't need to process anymore.
			if (m_stateByType.Count == 0)
				return;

			// if no state transition is required, just update current state
			if (m_NextState == null)
			{
				if (m_CurState != null)
					m_CurState.UpdateState();
				return;
			}

			// Switching the state
			m_PrevState = m_CurState;

			if( m_CurState != null )
				m_CurState.OnLeave();

			m_CurState = m_NextState;
			m_NextState = null;

			m_CurState.OnEnter();

			// update for first tick
			m_CurState.UpdateState();
		}

		public void Reset()
		{
			if (m_CurState != null)
				m_CurState.OnLeave();

			// reinitialize all state
			foreach (KeyValuePair<Type, State> state in m_stateByType)
			{
				//state.Value.Dispose();
				state.Value.Start();
			}
			//MemoryPoolManager.DestroyPooled(state.Value);

			//m_stateByType.Clear();

			m_CurState = null;
			m_NextState = null;
			m_PrevState = null;
		}
	}
}





