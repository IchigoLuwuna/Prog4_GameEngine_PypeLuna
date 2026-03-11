#ifndef COMMANDS_H
#define COMMANDS_H
#include "Command.h"
#include <string>
#include <glm/glm.hpp>

namespace dae
{
class TransformComponent;
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

class MoveCommand : public Command
{
public:
	MoveCommand( TransformComponent* pBoundTransform, const glm::vec2& movement );
	virtual ~MoveCommand() = default;

	virtual void Execute() override;

private:
	TransformComponent* m_pBoundTransform{};
	glm::vec2 m_Movement{};
};
} // namespace dae
#endif
