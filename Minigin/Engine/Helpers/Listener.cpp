#include "Listener.h"
#include "Engine/Core/Minigin.h"
#include "Engine/Core/EventManager.h"

dae::Listener::Listener( void* id, std::function<void( Event& event )> handler )
	: m_Id( id )
{
	Minigin::eventManager.AttachListener( m_Id, handler );
}

dae::Listener::~Listener()
{
	Minigin::eventManager.DetachListener( m_Id );
}

dae::Listener::Listener( Listener&& other )
	: m_Id( other.m_Id )
{
	if ( &other == this )
	{
		return;
	}

	other.m_Id = nullptr;
}

dae::Listener& dae::Listener::operator=( Listener&& other )
{
	if ( &other == this )
	{
		return *this;
	}

	m_Id = other.m_Id;
	other.m_Id = nullptr;

	return *this;
}
