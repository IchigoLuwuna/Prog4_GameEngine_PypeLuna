#include "StateComponent.h"

dae::StateComponent::StateComponent( GameObject* pParent )
	: Component( pParent )
	, m_State( pParent )
{
}

void dae::StateComponent::Update()
{
	m_State.Update();
}
