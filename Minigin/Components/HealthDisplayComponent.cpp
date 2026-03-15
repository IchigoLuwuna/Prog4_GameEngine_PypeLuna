#include "HealthDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Patterns/GameObject.h"
#include <cassert>
#include <format>

static const size_t healthChanged{ std::hash<std::string>()( "e_HealthChanged" ) };

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
	if ( eventHash != healthChanged )
	{
		return;
	}

	UpdateText( pHealth );
}

void dae::HealthDisplayComponent::UpdateText( HealthComponent* pHealth )
{
	auto health{ pHealth->GetHealth() };
	const std::string displayString{ std::format( "health: {}", health ) };
	m_pTextComponent->SetText( displayString );
}
