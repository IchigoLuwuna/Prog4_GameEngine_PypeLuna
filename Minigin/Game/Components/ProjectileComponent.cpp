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

dae::ProjectileComponent::~ProjectileComponent()
{
	m_Messenger.NotifyObservers( "e_ProjectileDelete"_hash );
}

void dae::ProjectileComponent::Update()
{
	m_Lifetime -= Timer::GetInstance().GetElapsed();
	if ( m_Lifetime <= 0.f )
	{
		GetParent()->MarkForRemoval();
		return;
	}

	auto transform{ GetParent()->GetComponent<dae::TransformComponent>() };
	transform->Move( m_Velocity * Timer::GetInstance().GetElapsed() );
}

void dae::ProjectileComponent::RemoveObserver( Observer<ProjectileComponent>* pObserver )
{
	m_Messenger.RemoveObserver( pObserver );
}
