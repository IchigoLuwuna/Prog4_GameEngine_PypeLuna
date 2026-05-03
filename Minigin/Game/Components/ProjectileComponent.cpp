#include "ProjectileComponent.h"
#include <Helpers.h>
#include <Components.h>

dae::ProjectileComponent::ProjectileComponent( GameObject* pParent, const glm::vec2& velocity, float lifetime )
	: Component( pParent )
	, m_Velocity( velocity )
	, m_Lifetime( lifetime )
	, m_Messenger( this )
{
}

void dae::ProjectileComponent::Update()
{
	m_Lifetime -= Timer::GetInstance().GetElapsed();
	if ( m_Lifetime <= 0.f )
	{
		GetParent()->MarkForRemoval();
		m_Messenger.NotifyObservers( "e_ProjectileTimeout"_hash );
		return;
	}

	GetParent()->GetComponent<dae::TransformComponent>()->Move( m_Velocity * Timer::GetInstance().GetElapsed() );
}
