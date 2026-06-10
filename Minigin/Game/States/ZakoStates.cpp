#include "ZakoStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Components.h>
#include <Sound.h>
#include <Random.h>

dae::ZakoIdlingState::ZakoIdlingState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::ZakoIdlingState::Update()
{
	m_RemainingStateTime -= Timer::GetInstance().GetElapsed();

	if ( m_RemainingStateTime < 0.f )
	{
		return GetParent()->FindOrCreateState<ZakoDivingState>();
	}

	return nullptr;
}

void dae::ZakoIdlingState::Enter()
{
	m_RemainingStateTime = dae::random::GetRand( m_MinStateTime, m_MaxStateTime );
}
void dae::ZakoIdlingState::Exit()
{
}

dae::ZakoDivingState::ZakoDivingState( StateMachine* pParent )
	: State( pParent )
{
}

void dae::ZakoDivingState::Enter()
{
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "dive.wav", 1.f );
	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	m_DivingTime = 0.f;
}
void dae::ZakoDivingState::Exit()
{
	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	auto center{ 288.f / 2.f };
	transform->MoveTo( center, -64.f );
}

dae::State* dae::ZakoDivingState::Update()
{
	m_DivingTime += Timer::GetInstance().GetElapsed();

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	auto movement{ glm::vec2{ std::cos( m_DivingTime * 6.28f ) * 64.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 224.f )
	{
		return GetParent()->FindOrCreateState<ZakoReturningState>();
	}

	return nullptr;
}

dae::ZakoReturningState::ZakoReturningState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::ZakoReturningState::Update()
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
	return GetParent()->FindOrCreateState<ZakoIdlingState>();
}

void dae::ZakoReturningState::Enter()
{
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<ZakoBrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}
}
void dae::ZakoReturningState::Exit()
{
}
