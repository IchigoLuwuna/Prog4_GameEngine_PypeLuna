#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H
#include <glm/glm.hpp>
#include <Patterns.h>
#include <Memory.h>
#include <Patterns.h>

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
