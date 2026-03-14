#include "MovementComponents.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

dae::SlideMovementComponent::SlideMovementComponent( GameObject* pParent, const glm::vec3& movement )
	: Component( pParent )
	, m_Movement( movement )
{
}

void dae::SlideMovementComponent::Update()
{
	GetParent()->GetComponent<TransformComponent>()->Move( m_Movement * Timer::GetInstance().GetElapsed() );
}

dae::OrbitMovementComponent::OrbitMovementComponent( GameObject* pParent,
													 float distance,
													 float speed,
													 float startingAngle )
	: Component( pParent )
	, m_Distance( distance )
	, m_Speed( speed )
	, m_Angle( startingAngle )
{
}

void dae::OrbitMovementComponent::Update()
{
	m_Angle += m_Speed * Timer::GetInstance().GetElapsed();

	glm::vec3 newPos{ std::cos( m_Angle ) * m_Distance, -std::sin( m_Angle ) * m_Distance, 0.f };

	GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( newPos );
}
