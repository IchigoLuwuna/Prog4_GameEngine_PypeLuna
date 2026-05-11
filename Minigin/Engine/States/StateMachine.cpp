#include "StateMachine.h"

void dae::StateMachine::Update( GameObject* pObject )
{
	State* pNewState{ m_CurrentState->Update( pObject ) };
	if ( pNewState )
	{
		SetState( pNewState );
	}
}

void dae::StateMachine::SetState( State* pNewState )
{
	if ( m_CurrentState )
	{
		m_CurrentState->Exit();
	}
	m_CurrentState = pNewState;
	m_CurrentState->Enter();
}
