#include "ObserverComponent.h"

dae::ObserverComponent::ObserverComponent( GameObject* pParent )
	: Component( pParent )
{
}

dae::ObserverComponent& dae::ObserverComponent::AddCallback( size_t eventHash, std::function<void( void* )> callback )
{
	m_ObserverCallbacks[eventHash] = callback;
	return *this;
}
dae::ObserverComponent& dae::ObserverComponent::RemoveCallback( size_t eventHash )
{
	m_ObserverCallbacks.erase( eventHash );
	return *this;
}

void dae::ObserverComponent::Notify( size_t eventHash, void* pSubject )
{
	if ( !m_ObserverCallbacks.contains( eventHash ) )
	{
		return;
	}

	m_ObserverCallbacks[eventHash]( pSubject );
}
