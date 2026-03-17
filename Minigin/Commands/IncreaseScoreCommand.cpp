#include "IncreaseScoreCommand.h"
#include "Components/ScoreComponent.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Helpers/SdbmHash.h"

dae::IncreaseScoreCommand::IncreaseScoreCommand( ScoreComponent* pBoundScore, uint32_t increase )
	: m_pBoundScore( pBoundScore )
	, m_Increase( increase )
	, m_Listener( this, std::bind( &IncreaseScoreCommand::HandleEvent, this, std::placeholders::_1 ) )
{
}

void dae::IncreaseScoreCommand::Execute()
{
	if ( m_pBoundScore )
	{
		m_pBoundScore->Accumulate( m_Increase );
	}
}

void dae::IncreaseScoreCommand::HandleEvent( Event& event )
{
	switch ( event.EventHash )
	{
	case Hash( "e_ComponentRemoved" ): {
		if ( event.pData == m_pBoundScore )
		{
			m_pBoundScore = nullptr;
		}
		return;
	}
	default: {
		return;
	}
	}
}
