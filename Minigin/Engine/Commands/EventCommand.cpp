#include "EventCommand.h"
#include "Core/Minigin.h"

dae::EventCommand::EventCommand( const Event& event )
	: m_Event( event )
{
}

void dae::EventCommand::Execute()
{
	Minigin::eventManager.SendEvent( m_Event );
}
