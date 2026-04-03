#include "DamageCommand.h"
#include "Components/HealthComponent.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"

dae::DamageCommand::DamageCommand( HealthComponent* pBoundHealth, uint32_t damage )
	: m_pBoundHealth( pBoundHealth )
	, m_Damage( damage )
	, m_Listener( this, std::bind( &DamageCommand::HandleEvent, this, std::placeholders::_1 ) )
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

void dae::DamageCommand::HandleEvent( Event& event )
{
	switch ( event.EventHash )
	{
	case "e_ComponentRemoved"_hash: {
		if ( event.pData == m_pBoundHealth )
		{
			m_pBoundHealth = nullptr;
		}
		return;
	}
	default: {
		return;
	}
	}
}
