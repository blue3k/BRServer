////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : finite state machine class
//
////////////////////////////////////////////////////////////////////////////////

using System.Collections;
using System.Collections.Generic;
using System;
using BR.GameSystem;

namespace BR
{
	public class FSM
	{
		// state class
		public abstract class State : IDisposable
		{

			public State( BRGame game, FSM fsm )
			{
                Game = game;
                MyFSM = fsm;
				IsOverlaped 	= false;
			}


            public BRGame Game { get; private set; }
            public FSM MyFSM { get; private set; }
			public bool IsOverlaped		{ get; set; }

			public virtual void Awake() { }
			public virtual void Dispose() { }
			public abstract void OnEnter();
			public abstract void OnLeave();
			public virtual void UpdateState() {
			}

//			public virtual bool OnMessage(Message message)
//			{
//				return false;
//			}
		}



		// state list
		private Dictionary<Type, State> m_StatesByType = new Dictionary<Type, State>();
		private     State m_CurrentState = null;
        private State m_NextState = null;
		protected   State m_PreviousState = null;


        public FSM()
        {
        }
		
		public bool Initialize()
		{
            Terminate();

			m_CurrentState = null;
            m_NextState = null;

            return true;
		}

        public void Terminate()
		{
			m_StatesByType.Clear();

			if (m_CurrentState != null)
			{
				m_CurrentState.OnLeave();
				m_CurrentState = null;
			}
			m_PreviousState = null;
		}

		// register state
		public virtual StateType CreateState<StateType>(BRGame game) where StateType : State
		{
			// already registered?
			if (m_StatesByType.ContainsKey(typeof(StateType)))
				throw new ArgumentException();

            StateType state = Activator.CreateInstance(typeof(StateType),game,this) as StateType;

			Type curType = state.GetType();
			while (curType != null
			       && !(curType.Equals(typeof(State)))
			       && !m_StatesByType.ContainsKey(curType))
			{
				m_StatesByType.Add(curType, state);
				curType = curType.BaseType;
			}

			state.Awake();

			return state;
		}

		//private void SetPreviousState<StateType>() where StateType : State
		//{
		//    m_PreviousState = stateID;
		//}
		
        public void RequestStateChange<NextState>() where NextState : State
		{
            RequestStateChange(typeof(NextState));
		}

        public void RequestStateChange(Type type)
        {
            m_NextState = m_StatesByType[type];
        }


		// Set next state
		public void SetNextState<NextState>() where NextState : State
		{
            SetNextState(typeof(NextState));
		}

		public virtual void SetNextState(Type type)
		{
            if (m_CurrentState != null)
            {
                m_CurrentState.OnLeave();
            }
                
            m_PreviousState = m_CurrentState;

            m_CurrentState = m_StatesByType[type];

            if (m_CurrentState != null)
            {
                m_CurrentState.OnEnter();
            }
		}

        public void BackToPreviousState()
        {
            if (m_PreviousState == null)
            {
                BRDebug.Warning("Previous state is null!");
                return; 
            }

            SetNextState(m_PreviousState.GetType());
        }

        public State GetCurState()
        {
            return m_CurrentState;
        }

		public StateType GetCurState<StateType>() where StateType : State
		{
			if( m_CurrentState is StateType )
				return m_CurrentState as StateType;

			return default(StateType);
		}

		public StateType GetState<StateType>() where StateType : State
		{
			if( m_StatesByType.ContainsKey(typeof(StateType)) )
				return m_StatesByType[typeof(StateType)] as StateType;

			return default(StateType);
		}

		// update fsm, current state update called
		public virtual void UpdateFSM()
		{
            if (m_NextState != null)
            {
                if (m_CurrentState != null)
                {
                    m_CurrentState.OnLeave();
                }

                m_PreviousState = m_CurrentState;

                m_CurrentState = m_NextState;
                m_NextState = null;

                if (m_CurrentState != null)
                {
                    m_CurrentState.OnEnter();
                }
            }

            if (null == m_CurrentState)
            {
                return;
            }
            
            m_CurrentState.UpdateState();
		}
	}
}




