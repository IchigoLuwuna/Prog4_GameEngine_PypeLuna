#include "HealthDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"
#include <cassert>
#include <format>

dae::HealthDisplayComponent::HealthDisplayComponent( GameObject* pParent, HealthComponent* pHealth )
	: Component( pParent )
{
	pHealth->RegisterObserver( this );

	if ( !m_pTextComponent )
	{
		m_pTextComponent = GetParent()->GetComponent<TextComponent>();
		assert( m_pTextComponent && "HealthDisplayComponent requires parent to have a text component" );
	}

	UpdateText( pHealth );
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
		m_pTextComponent->SetText( "dead :(" );
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
	m_pTextComponent->SetText( displayString );
}
