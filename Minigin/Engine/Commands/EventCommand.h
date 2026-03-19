#ifndef EVENTCOMMAND_H
#define EVENTCOMMAND_H
#include "Engine/Patterns/Command.h"
#include "Engine/Core/EventManager.h"

namespace dae
{
class EventCommand : public Command
{
public:
	EventCommand( const Event& event );
	virtual ~EventCommand() = default;
	virtual void Execute() override;

private:
	Event m_Event;
};
} // namespace dae
#endif
