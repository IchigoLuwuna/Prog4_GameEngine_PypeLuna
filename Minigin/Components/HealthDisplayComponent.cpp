#include "HealthDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"
#include <cassert>
#include <format>

dae::HealthDisplayComponent::HealthDisplayComponent( GameObject* pParent, HealthComponent* pHealth )
	: Component( pParent )
	, m_pSubject( pHealth )
{
	pHealth->RegisterObserver( this );

	if ( !m_pText )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText && "HealthDisplayComponent requires parent to have a text component" );
	}

	UpdateText( pHealth );
}

dae::HealthDisplayComponent::~HealthDisplayComponent()
{
	m_pSubject->RemoveObserver( this );
}

void dae::HealthDisplayComponent::Notify( size_t eventHash, HealthComponent* pHealth )
{
	switch ( eventHash )
	{
	case Hash( "e_HealthChanged" ): {
		UpdateText( pHealth );
		return;
	}
	case Hash( "e_EntityDied" ): {
		m_pText->SetText( "dead :(" );
	}
	default: {
		return;
	}
	}
}

void dae::HealthDisplayComponent::UpdateText( HealthComponent* pHealth )
{
	auto health{ pHealth->GetHealth() };
	const std::string displayString{ std::format( "health: {}", health ) };
	m_pText->SetText( displayString );
}
