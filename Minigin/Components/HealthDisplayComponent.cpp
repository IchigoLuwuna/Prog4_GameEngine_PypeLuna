#include "HealthDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"
#include <cassert>
#include <format>

dae::HealthDisplayComponent::HealthDisplayComponent( GameObject* pParent, HealthComponent* pHealth )
	: Component( pParent )
	, m_pSubject( pHealth )
	, m_Listener( this, std::bind( &HealthDisplayComponent::HandleEvent, this, std::placeholders::_1 ) )
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
	if ( m_pSubject )
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

void dae::HealthDisplayComponent::HandleEvent( Event& event )
{
	switch ( event.EventHash )
	{
	case "e_ComponentRemoved"_hash: {
		if ( event.pData == m_pSubject )
		{
			m_pSubject = nullptr;
		}
		return;
	}
	default: {
		return;
	}
	}
}
