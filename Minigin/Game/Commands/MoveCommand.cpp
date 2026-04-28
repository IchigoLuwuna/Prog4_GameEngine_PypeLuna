#include "MoveCommand.h"
#include "Engine/Components/TransformComponent.h"
#include "Engine/Helpers/Timer.h"

dae::MoveCommand::MoveCommand( const ReferencePtr<TransformComponent>& pBoundTransform, const glm::vec2& movement )
	: m_pBoundTransform( pBoundTransform )
	, m_Movement( movement )
{
}

void dae::MoveCommand::Execute()
{
	if ( !m_pBoundTransform.Validate() )
	{
		return;
	}

	m_pBoundTransform->Move( m_Movement * Timer::GetInstance().GetElapsed() );
}
