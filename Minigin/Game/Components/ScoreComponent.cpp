#include "ScoreComponent.h"
#include <Core.h>

dae::ScoreComponent::ScoreComponent( GameObject* pParent, uint32_t startingScore )
	: Component( pParent )
	, m_Score( startingScore )
	, m_Messenger( this )
	, m_Subscription( this, std::bind( &ScoreComponent::HandleEvent, this, std::placeholders::_1 ) )
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
	switch ( event.eventHash )
	{
	case "e_SmallPelletPickup"_hash: {
		if ( event.pData != GetParent() )
		{
			return;
		}

		Accumulate( 10 );

		return;
	}
	case "e_BigPelletPickup"_hash: {
		if ( event.pData != GetParent() )
		{
			return;
		}

		Accumulate( 100 );

		return;
	}
	default: {
		return;
	}
	}
}
