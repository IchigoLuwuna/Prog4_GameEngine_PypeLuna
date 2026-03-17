#include "MoveCommand.h"
#include "Engine/Core/EventManager.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Helpers/SdbmHash.h"
#include "Engine/Helpers/Timer.h"

dae::MoveCommand::MoveCommand( TransformComponent* pBoundTransform, const glm::vec2& movement )
	: m_pBoundTransform( pBoundTransform )
	, m_Movement( movement )
	, m_Listener( this, std::bind( &MoveCommand::HandleEvent, this, std::placeholders::_1 ) )
{
}

void dae::MoveCommand::Execute()
{
	if ( !m_pBoundTransform )
	{
		return;
	}

	m_pBoundTransform->Move( m_Movement * Timer::GetInstance().GetElapsed() );
}

void dae::MoveCommand::HandleEvent( Event& event )
{
	switch ( event.EventHash )
	{
	case Hash( "e_ComponentRemoved" ): {
		if ( event.pData == m_pBoundTransform )
		{
			m_pBoundTransform = nullptr;
		}
		return;
	}
	default: {
		return;
	}
	}
}
