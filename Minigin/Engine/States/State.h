#ifndef STATE_H
#define STATE_H
namespace dae
{
class StateMachine;
class GameObject;

class State
{
public:
	State( StateMachine* pParent )
		: m_pParent( pParent )
	{
	}
	virtual ~State() = default;

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual State* Update() = 0;

	StateMachine* GetParent()
	{
		return m_pParent;
	}

private:
	StateMachine* m_pParent{};
};
} // namespace dae
#endif
