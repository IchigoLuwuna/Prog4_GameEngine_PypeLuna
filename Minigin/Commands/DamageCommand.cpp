#include "DamageCommand.h"
#include "Components/HealthComponent.h"

dae::DamageCommand::DamageCommand( HealthComponent* pBoundHealth, uint32_t damage )
	: m_pBoundHealth( pBoundHealth )
	, m_Damage( damage )
{
}

void dae::DamageCommand::Execute()
{
	if ( !m_pBoundHealth )
	{
		return;
	}

	m_pBoundHealth->Damage( m_Damage );
	if ( m_pBoundHealth->IsDead() )
	{
		m_pBoundHealth = nullptr;
	}
}
