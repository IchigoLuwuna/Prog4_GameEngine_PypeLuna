#include "ScoreComponent.h"
#include <Core.h>

dae::ScoreComponent::ScoreComponent( GameObject* pParent,
									 const std::vector<std::pair<size_t, uint32_t>>& scoreGainOnEvents,
									 uint32_t startingScore )
	: Component( pParent )
	, m_Messenger( this )
	, m_Subscription( this, std::bind( &ScoreComponent::HandleEvent, this, std::placeholders::_1 ) )
	, m_ScoreGainOnEvents( scoreGainOnEvents )
	, m_Score( startingScore )
{
}
dae::ScoreComponent::ScoreComponent( GameObject* pParent,
									 std::vector<std::pair<size_t, uint32_t>>&& scoreGainOnEvents,
									 uint32_t startingScore )
	: Component( pParent )
	, m_Messenger( this )
	, m_Subscription( this, std::bind( &ScoreComponent::HandleEvent, this, std::placeholders::_1 ) )
	, m_ScoreGainOnEvents( std::move( scoreGainOnEvents ) )
	, m_Score( startingScore )
{
}

void dae::ScoreComponent::Accumulate( uint32_t increase )
{
	m_Score += increase;
	constexpr auto eventHash{ "e_ScoreChanged"_hash };
	m_Messenger.NotifyObservers( eventHash );
}

uint32_t dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

void dae::ScoreComponent::RemoveObserver( Observer<ScoreComponent>* pObserver )
{
	m_Messenger.RemoveObserver( pObserver );
}

void dae::ScoreComponent::HandleEvent( Event& event )
{
	for ( auto& scoreGainOnEvent : m_ScoreGainOnEvents )
	{
		if ( event.eventHash == scoreGainOnEvent.first )
		{
			Accumulate( scoreGainOnEvent.second );
		}
	}
}
