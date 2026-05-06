#ifndef STATECOMPONENT_H
#define STATECOMPONENT_H
#include <Patterns.h>
#include <States.h>

namespace dae
{
template <typename StateType>
	requires std::derived_from<StateType, State>
class StateComponent final : public Component
{
public:
	StateComponent( GameObject* pParent )
		: Component( pParent )
	{
	}
	virtual void Update() override
	{
		m_State->Update( GetParent() );
	}

	template <typename NewStateType>
		requires std::derived_from<NewStateType, StateType>
	void SetState()
	{
		m_State.template SetState<NewStateType>();
	}

private:
	StateMachine<StateType> m_State{};
};
} // namespace dae
#endif
