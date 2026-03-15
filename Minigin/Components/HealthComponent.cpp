#include "HealthComponent.h"
#include <algorithm>
#include <string>

static const size_t healthChanged{ std::hash<std::string>()( "e_HealthChanged" ) };

dae::HealthComponent::HealthComponent( GameObject* pGameObject, uint32_t health, uint32_t maxHealth )
	: Component( pGameObject )
	, m_Health( health )
	, m_MaxHealth( maxHealth )
	, m_Subject( this )
{
	if ( maxHealth == 0 )
	{
		m_MaxHealth = m_Health;
	}
}

void dae::HealthComponent::Heal( uint32_t healing )
{
	uint32_t newHealth{ m_Health + healing };
	m_Health = std::min( newHealth, m_MaxHealth );

	m_Subject.NotifyObservers( healthChanged );
}
void dae::HealthComponent::Damage( uint32_t damage )
{
	int32_t newHealth{ static_cast<int32_t>( m_Health ) -
					   static_cast<int32_t>( damage ) }; // signed to prevent underflows
	m_Health = std::max( 0, newHealth );

	m_Subject.NotifyObservers( healthChanged );
}

void dae::HealthComponent::IncreaseMax( uint32_t increase )
{
	m_MaxHealth += increase;
	m_Health += increase;

	m_Subject.NotifyObservers( healthChanged );
}
void dae::HealthComponent::DecreaseMax( uint32_t decrease )
{
	int32_t newMax{ static_cast<int32_t>( m_MaxHealth ) -
					static_cast<int32_t>( decrease ) }; // signed to prevent underflows
	m_MaxHealth = std::max( 1, newMax );
	m_Health = std::min( m_Health, m_MaxHealth );

	m_Subject.NotifyObservers( healthChanged );
}

uint32_t dae::HealthComponent::GetHealth() const
{
	return m_Health;
}
uint32_t dae::HealthComponent::GetMaxHealth() const
{
	return m_MaxHealth;
}
bool dae::HealthComponent::IsDead() const
{
	return m_Health != 0;
}

void dae::HealthComponent::RegisterObserver( Observer<HealthComponent>* pObserver )
{
	m_Subject.RegisterObserver( pObserver );
}
void dae::HealthComponent::RemoveObserver( Observer<HealthComponent>* pObserver )
{
	m_Subject.RemoveObserver( pObserver );
}
