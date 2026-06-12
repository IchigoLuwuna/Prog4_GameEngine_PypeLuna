#include "LivesComponent.h"

dae::LivesComponent::LivesComponent( GameObject* pParent, uint32_t startingLives )
	: Component( pParent )
	, m_Lives( startingLives )
{
}

uint32_t dae::LivesComponent::GetLives() const
{
	return m_Lives;
}

void dae::LivesComponent::SetLives( uint32_t newAmount )
{
	m_Lives = newAmount;
}
void dae::LivesComponent::ReduceLives( uint32_t amount )
{
	m_Lives += amount;
}
void dae::LivesComponent::IncreaseLives( uint32_t amount )
{
	m_Lives -= amount;
}
