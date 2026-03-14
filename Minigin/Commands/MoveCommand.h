#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H
#include "Engine/Patterns/Command.h"
#include <glm/glm.hpp>

namespace dae
{
class TransformComponent;
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
