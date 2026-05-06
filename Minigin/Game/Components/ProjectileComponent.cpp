#include "ProjectileComponent.h"
#include <Helpers.h>
#include <Components.h>

dae::ProjectileComponent::ProjectileComponent( GameObject* pParent, const glm::vec2& velocity, float lifetime )
	: Component( pParent )
	, m_Hitbox( pParent,
				{ 0.f, 0.f, 8.f, 8.f },
				{ "target_Enemy"_hash },
				[&]( auto, auto ) {
					m_Messenger.NotifyObservers( "e_ProjectileHit"_hash );
					GetParent()->MarkForRemoval();
				} )
	, m_Velocity( velocity )
	, m_Lifetime( lifetime )
	, m_Messenger( this )
{
	auto transform{ GetParent()->GetComponent<dae::TransformComponent>() };
	glm::vec4 newHitboxRect{
		transform->GetPosition().x,
		transform->GetPosition().y,
		m_Hitbox.m_Rectangle.z,
		m_Hitbox.m_Rectangle.w,
	};

	m_Hitbox.m_Rectangle = newHitboxRect;
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

	auto transform{ GetParent()->GetComponent<dae::TransformComponent>() };
	transform->Move( m_Velocity * Timer::GetInstance().GetElapsed() );

	glm::vec4 newHitboxRect{
		transform->GetPosition().x,
		transform->GetPosition().y,
		m_Hitbox.m_Rectangle.z,
		m_Hitbox.m_Rectangle.w,
	};
	m_Hitbox.m_Rectangle = newHitboxRect;
}

void dae::ProjectileComponent::RemoveObserver( Observer<ProjectileComponent>* pObserver )
{
	m_Messenger.RemoveObserver( pObserver );
}
