#include "ScoreDisplayComponent.h"
#include <cassert>
#include <format>
#include "Components/ScoreComponent.h"
#include "Components/TextComponent.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent( GameObject* pParent, ScoreComponent* pScore )
	: Component( pParent )
	, m_pSubject( pScore )
	, m_Listener( this, std::bind( &ScoreDisplayComponent::HandleEvent, this, std::placeholders::_1 ) )
{
	pScore->RegisterObserver( this );

	if ( !m_pText )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText && "ScoreDisplayComponent requires parent to have a text component" );
	}

	UpdateText( pScore );
}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if ( m_pSubject )
	{
		m_pSubject->RemoveObserver( this );
	}
}

void dae::ScoreDisplayComponent::Notify( size_t eventHash, ScoreComponent* pSubject )
{
	switch ( eventHash )
	{
	case Hash( "e_ScoreChanged" ): {
		UpdateText( pSubject );
	}
	default: {
		return;
	}
	}
}

void dae::ScoreDisplayComponent::UpdateText( ScoreComponent* pScore )
{
	auto score{ pScore->GetScore() };
	const std::string displayString{ std::format( "score: {}", score ) };
	m_pText->SetText( displayString );
}

void dae::ScoreDisplayComponent::HandleEvent( Event& event )
{
	switch ( event.EventHash )
	{
	case Hash( "e_ComponentRemoved" ): {
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
