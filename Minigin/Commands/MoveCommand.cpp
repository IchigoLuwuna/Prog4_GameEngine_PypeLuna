#include "MoveCommand.h"
#include "Components/TransformComponent.h"
#include "Timer.h"

dae::MoveCommand::MoveCommand( TransformComponent* pBoundTransform, const glm::vec2& movement )
	: m_pBoundTransform( pBoundTransform )
	, m_Movement( movement )
{
}

void dae::MoveCommand::Execute()
{
	m_pBoundTransform->Move( m_Movement * Timer::GetInstance().GetElapsed() );
}
