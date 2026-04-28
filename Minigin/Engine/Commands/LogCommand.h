#ifndef LOGCOMMAND_H
#define LOGCOMMAND_H
#include "Patterns/Command.h"
#include <string>

namespace dae
{
class LogCommand : public Command // For testing purposes
{
public:
	LogCommand( const std::string& string );
	LogCommand( std::string&& string );
	virtual ~LogCommand() = default;

	virtual void Execute() override;

private:
	std::string m_Text{};
};
} // namespace dae

#endif
