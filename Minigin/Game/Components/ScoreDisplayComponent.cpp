#include "ScoreDisplayComponent.h"
#include <cassert>
#include <format>
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Patterns/GameObject.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent( GameObject* pParent, const ReferencePtr<ScoreComponent>& pScore )
	: Component( pParent )
	, m_pSubject( pScore )
{
	pScore->RegisterObserver( this );

	if ( !m_pText.Validate() )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText.Validate() && "ScoreDisplayComponent requires parent to have a text component" );
	}

	UpdateText( pScore.Get() );
}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if ( m_pSubject.Validate() )
	{
		m_pSubject->RemoveObserver( this );
	}
}

void dae::ScoreDisplayComponent::Notify( size_t eventHash, ScoreComponent* pSubject )
{
	switch ( eventHash )
	{
	case "e_ScoreChanged"_hash: {
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
