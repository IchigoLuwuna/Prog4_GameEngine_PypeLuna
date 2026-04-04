#include "HealthDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"
#include <cassert>
#include <format>

dae::HealthDisplayComponent::HealthDisplayComponent( GameObject* pParent, const ReferencePtr<HealthComponent>& pHealth )
	: Component( pParent )
	, m_pSubject( pHealth )
{
	pHealth->RegisterObserver( this );

	if ( !m_pText.Validate() )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText.Validate() && "HealthDisplayComponent requires parent to have a text component" );
	}

	UpdateText( pHealth.Get() );
}

dae::HealthDisplayComponent::~HealthDisplayComponent()
{
	if ( m_pSubject.Validate() )
	{
		m_pSubject->RemoveObserver( this );
	}
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
