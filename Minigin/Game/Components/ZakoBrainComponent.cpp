#include "ZakoBrainComponent.h"

dae::ZakoBrainComponent::ZakoBrainComponent( GameObject* pParent )
	: Component( pParent )
	, m_FormationSlot( m_HiveMind.GetFormationSlot( HiveMind::HiveMindType::zako ) )
{
}

dae::ZakoBrainComponent::~ZakoBrainComponent()
{
	m_HiveMind.ClearFormationSlot( m_FormationSlot );
}

glm::vec2 dae::ZakoBrainComponent::GetFormationPosition() const
{
	return m_HiveMind.GetFormationPosition( m_FormationSlot );
}

dae::GameObject* dae::ZakoBrainComponent::GetPlayer()
{
	return m_HiveMind.GetPlayer();
}
