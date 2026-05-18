#include "StateMachine.h"

dae::StateMachine::StateMachine( GameObject* pParent )
	: m_pParent( pParent )
{
}

void dae::StateMachine::Update()
{
	State* pNewState{ m_CurrentState->Update() };
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

dae::GameObject* dae::StateMachine::GetParent()
{
	return m_pParent;
}
