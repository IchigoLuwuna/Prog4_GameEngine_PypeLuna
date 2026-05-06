#ifndef STATE_H
#define STATE_H
namespace dae
{
template <typename DerivedState>
class StateMachine;

template <typename DerivedState>
class State
{
public:
	State( StateMachine<DerivedState>* pParent )
		: m_pParent( pParent )
	{
	}

	virtual void Enter() = 0;
	virtual void Exit() = 0;

	StateMachine<DerivedState>* GetParent()
	{
		return m_pParent;
	}

private:
	StateMachine<DerivedState>* m_pParent{};
};
} // namespace dae
#endif
