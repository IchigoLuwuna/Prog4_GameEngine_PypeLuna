#include "StateComponent.h"

dae::StateComponent::StateComponent( GameObject* pParent )
	: Component( pParent )
{
}

void dae::StateComponent::Update()
{
	m_State.Update( GetParent() );
}
