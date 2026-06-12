#include "BrainComponent.h"

dae::BrainComponent::BrainComponent( GameObject* pParent, HiveMind::HiveMindType type )
	: Component( pParent )
	, m_FormationSlot( m_HiveMind.GetFormationSlot( type ) )
{
}

dae::BrainComponent::~BrainComponent()
{
	m_HiveMind.ClearFormationSlot( m_FormationSlot );
}

glm::vec2 dae::BrainComponent::GetFormationPosition() const
{
	return m_HiveMind.GetFormationPosition( m_FormationSlot );
}

dae::GameObject* dae::BrainComponent::GetPlayer()
{
	return m_HiveMind.GetPlayer();
}

void dae::BrainComponent::SetTractoring()
{
	m_HiveMind.SetTractoring();
}
void dae::BrainComponent::ClearTractoring()
{
	m_HiveMind.ClearTractoring();
}
bool dae::BrainComponent::IsTractoring()
{
	return m_HiveMind.IsTractoring();
}
