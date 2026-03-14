#include "LogCommand.h"
#include <iostream>

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
