#include "TractorComponent.h"
#include <Helpers.h>

dae::TractorComponent::TractorComponent( GameObject* pParent )
	: Component( pParent )
{
}

dae::TractorComponent::~TractorComponent()
{
}

void dae::TractorComponent::Update()
{
	if ( !m_HitboxRef.Validate() )
	{
		m_HitboxRef = GetParent()->GetComponent<dae::HitboxComponent>();
		assert( m_HitboxRef.Validate() && "Parent must have hitbox" );
	}

	m_TractoringTime += Timer::GetInstance().GetElapsed();
	constexpr float timePerPhase{ 0.51f };

	if ( m_TractoringTime > timePerPhase * 10 )
	{
		GetParent()->MarkForRemoval();
		return;
	}

	const int currentPhase{ static_cast<int>( std::floor( m_TractoringTime / timePerPhase ) ) };
	const float timeRounded{ currentPhase * timePerPhase };
	const glm::vec4 tractorFull{ 0.f, 0.f, 48.f, 80.f };
	const glm::vec4 tractorCurrentHitRect{
		tractorFull.x,
		tractorFull.y,
		tractorFull.z,
		tractorFull.w * ( ( timeRounded + timePerPhase ) / ( timePerPhase * 10 ) ),
	};

	m_HitboxRef->SetHitbox( tractorCurrentHitRect );

	if ( m_CurrentPhase == currentPhase )
	{
		return;
	}
}
