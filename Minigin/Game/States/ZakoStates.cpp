#include "ZakoStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Components.h>
#include <Sound.h>

dae::ZakoIdlingState::ZakoIdlingState( StateMachine<ZakoState>* pParent )
	: ZakoState( pParent )
{
}

void dae::ZakoIdlingState::Update( GameObject* )
{
	m_RemainingStateTime -= Timer::GetInstance().GetElapsed();

	if ( m_RemainingStateTime < 0.f )
	{
		GetParent()->SetState<ZakoDivingState>();
		return;
	}
}

void dae::ZakoIdlingState::Enter()
{
	m_RemainingStateTime = static_cast<float>( rand() ) / static_cast<float>( RAND_MAX ) * m_MaxStateTime;
}
void dae::ZakoIdlingState::Exit()
{
}

dae::ZakoDivingState::ZakoDivingState( StateMachine<ZakoState>* pParent )
	: ZakoState( pParent )
{
}

void dae::ZakoDivingState::Enter()
{
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "dive.wav", 1.f );
}
void dae::ZakoDivingState::Exit()
{
}

void dae::ZakoDivingState::Update( GameObject* pObject )
{
	auto transform{ pObject->GetComponent<dae::TransformComponent>() };
	auto movement{ glm::vec2{ 0.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 224.f )
	{
		transform->MoveTo( transform->GetPosition().x, -64.f );
		GetParent()->SetState<ZakoReturningState>();
		return;
	}
}

dae::ZakoReturningState::ZakoReturningState( StateMachine<ZakoState>* pParent )
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
		GetParent()->SetState<ZakoIdlingState>();
		return;
	}
}

void dae::ZakoReturningState::Enter()
{
}
void dae::ZakoReturningState::Exit()
{
}
