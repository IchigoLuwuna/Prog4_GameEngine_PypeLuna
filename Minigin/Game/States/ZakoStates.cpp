#include "ZakoStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Components.h>
#include <Sound.h>

dae::ZakoIdlingState::ZakoIdlingState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::ZakoIdlingState::Update( GameObject* )
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
	m_RemainingStateTime = static_cast<float>( rand() ) / static_cast<float>( RAND_MAX ) * m_MaxStateTime;
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
}
void dae::ZakoDivingState::Exit()
{
	if ( m_pParent )
	{
		auto transform{ m_pParent->GetComponent<dae::TransformComponent>() };
		transform->MoveTo( m_StartingX, transform->GetPosition().y );
	}

	m_DivingTime = 0.f;
	m_StartingXSet = false;
}

dae::State* dae::ZakoDivingState::Update( GameObject* pObject )
{
	m_DivingTime += Timer::GetInstance().GetElapsed();

	m_pParent = pObject;
	auto transform{ pObject->GetComponent<dae::TransformComponent>() };
	if ( !m_StartingXSet )
	{
		m_StartingX = transform->GetPosition().x;
		m_StartingXSet = true;
	}
	auto movement{ glm::vec2{ std::cos( m_DivingTime * 6.28f ) * 64.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 224.f )
	{
		transform->MoveTo( transform->GetPosition().x, -64.f );
		return GetParent()->FindOrCreateState<ZakoReturningState>();
	}

	return nullptr;
}

dae::ZakoReturningState::ZakoReturningState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::ZakoReturningState::Update( GameObject* pObject )
{
	auto transform{ pObject->GetComponent<dae::TransformComponent>() };
	auto movement{ glm::vec2{ 0.f, 128.f } * Timer::GetInstance().GetElapsed() };

	transform->Move( movement );
	if ( transform->GetPosition().y >= 8.f )
	{
		transform->MoveTo( transform->GetPosition().x, 8.f );
		return GetParent()->FindOrCreateState<ZakoIdlingState>();
	}

	return nullptr;
}

void dae::ZakoReturningState::Enter()
{
}
void dae::ZakoReturningState::Exit()
{
}
