#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#include <vector>
#include <memory>

namespace dae
{
template <typename BaseState>
class StateMachine
{
public:
	StateMachine() = default;

	template <typename NewState>
		requires std::derived_from<NewState, BaseState>
	void SetState()
	{
		if ( m_CurrentState )
		{
			m_CurrentState->Exit();
		}
		m_CurrentState = FindState<NewState>();
		if ( !m_CurrentState )
		{
			m_States.push_back( std::make_unique<NewState>( this ) );
			m_CurrentState = m_States.back().get();
		}
		m_CurrentState->Enter();
	}

	BaseState* operator->()
	{
		return m_CurrentState;
	}

private:
	std::vector<std::unique_ptr<BaseState>> m_States{};
	BaseState* m_CurrentState{};

	template <typename SearchedStateType>
		requires std::derived_from<SearchedStateType, BaseState>
	BaseState* FindState()
	{
		for ( auto& state : m_States )
		{
			SearchedStateType* pState{ dynamic_cast<SearchedStateType*>( state.get() ) };
			if ( pState )
			{
				return pState;
			}
		}

		return nullptr;
	}
};
} // namespace dae
#endif
