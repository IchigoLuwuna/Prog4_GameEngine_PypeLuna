#include "GameStateHandlerComponent.h"

dae::GameStateHandlerComponent::GameStateHandlerComponent( GameObject* pParent )
	: Component( pParent )
	, m_StateMachine( pParent )
{
}
