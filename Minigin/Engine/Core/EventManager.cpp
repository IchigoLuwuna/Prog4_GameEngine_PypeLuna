#include "EventManager.h"
#ifndef NDEBUG
#	include <iostream>
#endif

void dae::EventManager::Destroy()
{
	ProcessEvents();
	m_Listeners.clear();
}

void dae::EventManager::AttachListener( void* pListener, std::function<void( Event& event )> handler )
{
	m_Listeners.push_back( { pListener, handler } );
}

void dae::EventManager::DetachListener( void* pListener )
{
	std::erase_if( m_Listeners, [&]( auto& pCurrentListener ) { return pCurrentListener.first == pListener; } );
}

void dae::EventManager::ProcessEvents()
{
	for ( size_t eventIndex{}; eventIndex < m_EventsBack; ++eventIndex )
	{
		auto& event{ m_Events[eventIndex] };
		for ( auto& listener : m_Listeners )
		{
			auto& handler{ listener.second };
			handler( event );
		}
	}

	m_Events.fill( {} );
	m_EventsBack = 0;
}

void dae::EventManager::SendEvent( const Event& event )
{
	m_Events[m_EventsBack] = event;
	++m_EventsBack;

#ifndef NDEBUG
	if ( m_EventsBack >= m_Events.size() - 1 )
	{
		std::cout << "Warning: Event queue is full, consider expanding it\n";
	}
#endif
}
