#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <vector>
#include <memory>
#include "States/State.h"

namespace dae
{
class StateMachine
{
public:
	StateMachine() = default;

	template <typename SearchedStateType>
		requires std::derived_from<SearchedStateType, State>
	State* FindOrCreateState()
	{
		for ( auto& state : m_States )
		{
			SearchedStateType* pState{ dynamic_cast<SearchedStateType*>( state.get() ) };
			if ( pState )
			{
				return pState;
			}
		}

		m_States.push_back( std::make_unique<SearchedStateType>( this ) );
		return m_States.back().get();
	}

	void Update( GameObject* pObject );
	void SetState( State* pNewState );

private:
	std::vector<std::unique_ptr<State>> m_States{};
	State* m_CurrentState{};
};
} // namespace dae
#endif
