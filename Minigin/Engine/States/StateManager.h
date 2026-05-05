#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include <memory>
#include "State.h"

namespace dae
{
template <typename BaseState>
	requires std::derived_from<BaseState, State>
class StateManager
{
public:
	template <typename DerivedState>
		requires std::derived_from<DerivedState, BaseState>
	void SetState()
	{
		m_State = std::make_unique<DerivedState>( this );
	}

	BaseState* operator->()
	{
		return m_State.get();
	}

private:
	std::unique_ptr<BaseState> m_State{};
};
} // namespace dae
#endif
