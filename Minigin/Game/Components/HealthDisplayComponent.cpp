#include "HealthDisplayComponent.h"
#include "TextComponent.h"
#include <cassert>
#include <format>

dae::HealthDisplayComponent::HealthDisplayComponent( GameObject* pParent )
	: Component( pParent )
{
	if ( !m_pText.Validate() )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText.Validate() && "HealthDisplayComponent requires parent to have a text component" );
	}
}

dae::HealthDisplayComponent& dae::HealthDisplayComponent::SetSubjectHealth(
	const ReferencePtr<HealthComponent>& subject )
{
	subject->RegisterObserver( GetParent()->GetComponent<HealthDisplayComponent>() );
	return *this;
}

void dae::HealthDisplayComponent::Notify( size_t eventHash, HealthComponent* pHealth )
{
	switch ( eventHash )
	{
	case "e_HealthChanged"_hash: {
		UpdateText( pHealth );
		return;
	}
	case "e_EntityDied"_hash: {
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
