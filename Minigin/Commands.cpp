#include "Commands.h"
#include <iostream>
#include "TransformComponent.h"
#include "Timer.h"

dae::LogCommand::LogCommand( const std::string& string )
	: m_Text( string )
{
}

dae::LogCommand::LogCommand( std::string&& string )
	: m_Text( std::move( string ) )
{
}

void dae::LogCommand::Execute()
{
	std::cout << m_Text << "\n";
}

dae::MoveCommand::MoveCommand( TransformComponent* pBoundTransform, const glm::vec2& movement )
	: m_pBoundTransform( pBoundTransform )
	, m_Movement( movement )
{
}

void dae::MoveCommand::Execute()
{
	m_pBoundTransform->Move( m_Movement * Timer::GetInstance().GetElapsed() );
}
