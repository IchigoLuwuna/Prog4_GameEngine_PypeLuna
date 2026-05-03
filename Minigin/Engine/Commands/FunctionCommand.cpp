#include "FunctionCommand.h"

dae::FunctionCommand::FunctionCommand( std::function<void()> function )
	: m_Function( function )
{
}

void dae::FunctionCommand::Execute()
{
	m_Function();
}
