#include "ZakoStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Components.h>
#include <Sound.h>

dae::ZakoIdlingState::ZakoIdlingState( void* pParent )
	: ZakoState( pParent )
	, m_RemainingStateTime( static_cast<float>( rand() ) / static_cast<float>( RAND_MAX ) * m_MaxStateTime )
{
}

void dae::ZakoIdlingState::Update( GameObject* )
{
	m_RemainingStateTime -= Timer::GetInstance().GetElapsed();

	if ( m_RemainingStateTime < 0.f )
	{
		reinterpret_cast<StateManager<ZakoState>*>( GetParent() )->SetState<ZakoDivingState>();
		return;
	}
}

dae::ZakoDivingState::ZakoDivingState( void* pParent )
	: ZakoState( pParent )
{
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "dive.wav", 1.f );
}

void dae::ZakoDivingState::Update( GameObject* pObject )
{
	auto transform{ pObject->GetComponent<dae::TransformComponent>() };
	auto movement{ glm::vec2{ 0.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 224.f )
	{
		transform->MoveTo( transform->GetPosition().x, -64.f );
		reinterpret_cast<StateManager<ZakoState>*>( GetParent() )->SetState<ZakoReturningState>();
		return;
	}
}

dae::ZakoReturningState::ZakoReturningState( void* pParent )
	: ZakoState( pParent )
{
}

void dae::ZakoReturningState::Update( GameObject* pObject )
{
	auto transform{ pObject->GetComponent<dae::TransformComponent>() };
	auto movement{ glm::vec2{ 0.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 8.f )
	{
		transform->MoveTo( transform->GetPosition().x, 8.f );
		reinterpret_cast<StateManager<ZakoState>*>( GetParent() )->SetState<ZakoIdlingState>();
		return;
	}
}
