#include "BossStates.h"
#include "Components/TransformComponent.h"
#include "States/StateMachine.h"
#include <Helpers.h>

dae::BossReturningState::BossReturningState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::BossReturningState::Update()
{
	constexpr float speed{ 92.f };

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	auto target{ m_BrainRef->GetFormationPosition() };
	auto movement{ glm::normalize( target - transform->GetPosition() ) * speed };
	movement *= Timer::GetInstance().GetElapsed();

	transform->Move( movement );

	float epsilon{ speed * Timer::GetInstance().GetElapsed() * 1.1f }; // Dynamic snapping factor
	// Not at target pos
	if ( transform->GetPosition().x > target.x + epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().x < target.x - epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().y > target.y + epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().y < target.y - epsilon )
	{
		return nullptr;
	}
	//

	// Snap to target and change state to idle
	transform->MoveTo( target );
	return GetParent()->FindOrCreateState<BossIdlingState>();
}
void dae::BossReturningState::Enter()
{
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}
}
void dae::BossReturningState::Exit()
{
}
