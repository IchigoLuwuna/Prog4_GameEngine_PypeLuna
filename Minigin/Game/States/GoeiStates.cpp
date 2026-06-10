#include "GoeiStates.h"
#include <Helpers.h>
#include <Random.h>
#include <Components.h>
#include <Sound.h>

dae::GoeiIdlingState::GoeiIdlingState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::GoeiIdlingState::Update()
{
	m_RemainingStateTime -= Timer::GetInstance().GetElapsed();

	if ( m_RemainingStateTime < 0.f )
	{
		return GetParent()->FindOrCreateState<GoeiDivingState>();
	}

	return nullptr;
}

void dae::GoeiIdlingState::Enter()
{
	m_RemainingStateTime = dae::random::GetRand( m_MinStateTime, m_MaxStateTime );
}
void dae::GoeiIdlingState::Exit()
{
}

dae::GoeiDivingState::GoeiDivingState( StateMachine* pParent )
	: State( pParent )
{
}

void dae::GoeiDivingState::Enter()
{
	// Get Brain reference
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}

	// Verify there is a player to target
	auto pPlayer{ m_BrainRef->GetPlayer() };
	if ( !pPlayer )
	{
		m_PlayerFound = false;
		return;
	}
	m_PlayerFound = true;

	ServiceLocator<SoundService>::GetInstance().GetService().Play( "dive.wav", 1.f );
	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };

	// Set Bezier Path
	glm::vec2 a{ transform->GetPosition() };
	glm::vec2 b{ transform->GetPosition().x + 64.f, transform->GetPosition().y - 64.f };
	glm::vec2 c{ transform->GetPosition().x - 64.f, transform->GetPosition().y - 64.f };
	glm::vec2 d{ pPlayer->GetComponent<dae::TransformComponent>()->GetPosition() };
	bool RandomDivingWindingFlip{ dae::random::GetRand( 50 ) };
	if ( RandomDivingWindingFlip )
	{
		std::swap( b, c );
	}
	m_BezierPath.controlPoints = { a, b, c, d };
	m_DivingTime = 0.f;
}
void dae::GoeiDivingState::Exit()
{
}
dae::State* dae::GoeiDivingState::Update()
{
	m_DivingTime += Timer::GetInstance().GetElapsed();
	constexpr float divingTimeMax{ 2.f };
	const float lerp{ m_DivingTime / divingTimeMax };

	if ( m_DivingTime >= divingTimeMax + 0.5f )
	{
		return GetParent()->FindOrCreateState<GoeiSecondDiveState>();
	}

	if ( !m_PlayerFound )
	{
		return GetParent()->FindOrCreateState<GoeiReturningState>();
	}

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };

	auto lerpedPos{ m_BezierPath.Lerp( lerp ) };
	transform->MoveTo( lerpedPos.x + std::sin( m_DivingTime * 2.f ) * 32.f, lerpedPos.y );

	return nullptr;
}

dae::GoeiSecondDiveState::GoeiSecondDiveState( StateMachine* pParent )
	: State( pParent )
{
}
void dae::GoeiSecondDiveState::Enter()
{
	// Get Brain reference
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}

	// Verify there is a player to target
	auto pPlayer{ m_BrainRef->GetPlayer() };
	if ( !pPlayer )
	{
		m_PlayerFound = false;
		return;
	}
	m_PlayerFound = true;

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	auto playerTransform{ pPlayer->GetComponent<dae::TransformComponent>()->GetPosition() };

	// Set Bezier Path
	glm::vec2 a{ transform->GetPosition() };
	glm::vec2 b{ playerTransform.x + 64.f, playerTransform.y - 64.f };
	glm::vec2 c{ playerTransform.x + 64.f, playerTransform.y + 64.f };
	glm::vec2 d{ playerTransform };
	bool RandomDivingWindingFlip{ dae::random::GetRand( 50 ) };
	if ( RandomDivingWindingFlip )
	{
		b.x = playerTransform.x - 64.f;
		c.x = playerTransform.x - 64.f;
	}
	m_BezierPath.controlPoints = { a, b, c, d };
	m_DivingTime = 0.f;
}
void dae::GoeiSecondDiveState::Exit()
{
}
dae::State* dae::GoeiSecondDiveState::Update()
{
	m_DivingTime += Timer::GetInstance().GetElapsed();
	constexpr float divingTimeMax{ 2.5f };
	const float lerp{ m_DivingTime / divingTimeMax };

	if ( m_DivingTime >= divingTimeMax + 0.5f )
	{
		return GetParent()->FindOrCreateState<GoeiReturningState>();
	}

	if ( !m_PlayerFound )
	{
		return GetParent()->FindOrCreateState<GoeiReturningState>();
	}

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };

	transform->MoveTo( m_BezierPath.Lerp( lerp ) );

	return nullptr;
}

dae::GoeiReturningState::GoeiReturningState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::GoeiReturningState::Update()
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
	return GetParent()->FindOrCreateState<GoeiIdlingState>();
}

void dae::GoeiReturningState::Enter()
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
void dae::GoeiReturningState::Exit()
{
}
