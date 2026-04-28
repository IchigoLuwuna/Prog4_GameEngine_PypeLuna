#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H
#include "Engine/Helpers/Subscription.h"
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Command.h"
#include <glm/glm.hpp>

namespace dae
{
class TransformComponent;
struct Event;
class MoveCommand : public Command
{
public:
	MoveCommand( const ReferencePtr<TransformComponent>& pBoundTransform, const glm::vec2& movement );
	virtual ~MoveCommand() = default;

	virtual void Execute() override;

private:
	ReferencePtr<TransformComponent> m_pBoundTransform{};
	glm::vec2 m_Movement{};

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
