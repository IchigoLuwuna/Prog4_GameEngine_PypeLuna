#include "ScoreDisplayComponent.h"
#include <cassert>
#include <format>
#include "ScoreComponent.h"
#include "TextComponent.h"

dae::ScoreDisplayComponent::ScoreDisplayComponent( GameObject* pParent )
	: Component( pParent )
{
	if ( !m_pText.Validate() )
	{
		m_pText = GetParent()->GetComponent<TextComponent>();
		assert( m_pText.Validate() && "ScoreDisplayComponent requires parent to have a text component" );
		m_pText->SetText( "score\n" );
	}
}

dae::ScoreDisplayComponent& dae::ScoreDisplayComponent::SetSubjectScore( const ReferencePtr<ScoreComponent>& subject )
{
	subject->RegisterObserver( GetParent()->GetComponent<ScoreDisplayComponent>() );
	return *this;
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
	const std::string displayString{ std::format( "score\n{}", score ) };
	m_pText->SetText( displayString );
}
