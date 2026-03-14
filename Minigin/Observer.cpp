#include "Observer.h"
#include <algorithm>

void dae::Subject::RegisterObserver( ObserverBase* pObserver )
{
	m_Observers.push_back( pObserver );
}

void dae::Subject::RemoveObserver( ObserverBase* pObserver )
{
	m_Observers.erase( std::find( m_Observers.begin(), m_Observers.end(), pObserver ) );
}

void dae::Subject::NotifyObservers( size_t eventHash )
{
	for ( auto* observer : m_Observers )
	{
		observer->ForwardNotify( eventHash, this );
	}
}
