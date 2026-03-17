#include "ScoreComponent.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"

void dae::ScoreComponent::Accumulate( uint32_t increase )
{
	m_Score += increase;
	constexpr auto eventHash{ Hash( "e_ScoreChanged" ) };
	EventManager::GetInstance().SendEvent( { eventHash, &m_Score } );
	m_Subject.NotifyObservers( eventHash );
}

uint32_t dae::ScoreComponent::GetScore() const
{
	return m_Score;
}

void dae::ScoreComponent::RegisterObserver( Observer<ScoreComponent>* pObserver )
{
	m_Subject.RegisterObserver( pObserver );
}
void dae::ScoreComponent::RemoveObserver( Observer<ScoreComponent>* pObserver )
{
	m_Subject.RegisterObserver( pObserver );
}
