#include "Subscription.h"
#include "Engine/Core/Minigin.h"
#include "Engine/Core/EventManager.h"

dae::Subscription::Subscription( void* id, std::function<void( Event& event )> handler )
	: m_Id( id )
{
	Minigin::eventManager.AttachListener( m_Id, handler );
}

dae::Subscription::~Subscription()
{
	Minigin::eventManager.DetachListener( m_Id );
}

dae::Subscription::Subscription( Subscription&& other )
	: m_Id( other.m_Id )
{
	if ( &other == this )
	{
		return;
	}

	other.m_Id = nullptr;
}

dae::Subscription& dae::Subscription::operator=( Subscription&& other )
{
	if ( &other == this )
	{
		return *this;
	}

	m_Id = other.m_Id;
	other.m_Id = nullptr;

	return *this;
}
