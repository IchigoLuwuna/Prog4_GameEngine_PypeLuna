#ifndef GAMESTATEHANDLERCOMPONENT_H
#define GAMESTATEHANDLERCOMPONENT_H
#include "Patterns/Component.h"
#include "States/StateMachine.h"

namespace dae
{
class GameStateHandlerComponent : public Component
{
public:
	GameStateHandlerComponent( GameObject* pParent );

	virtual void Update() override
	{
		m_StateMachine.Update();
	}

	template <typename NewStateType>
		requires std::derived_from<NewStateType, State>
	void SetState()
	{
		m_StateMachine.SetState( m_StateMachine.FindOrCreateState<NewStateType>() );
	}

private:
	StateMachine m_StateMachine;
};
} // namespace dae
#endif
