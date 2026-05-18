#ifndef STATECOMPONENT_H
#define STATECOMPONENT_H
#include <Patterns.h>
#include <States.h>

namespace dae
{
class StateComponent final : public Component
{
public:
	StateComponent( GameObject* pParent );

	virtual void Update() override;

	template <typename NewStateType>
		requires std::derived_from<NewStateType, State>
	void SetState()
	{
		m_State.SetState( m_State.FindOrCreateState<NewStateType>() );
	}

private:
	StateMachine m_State;
};
} // namespace dae
#endif
