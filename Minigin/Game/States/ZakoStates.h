#ifndef ZAKOSTATES_H
#define ZAKOSTATES_H
#include <States.h>

namespace dae
{
// The bee guys
class GameObject;
class ZakoState : public State<ZakoState>
{
public:
	ZakoState( StateMachine<ZakoState>* pParent )
		: State( pParent )
	{
	}
	virtual ~ZakoState() = default;
	virtual void Update( GameObject* ) = 0;
};

class ZakoIdlingState final : public ZakoState
{
public:
	ZakoIdlingState( StateMachine<ZakoState>* pParent );
	virtual void Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	float m_RemainingStateTime{};

	constexpr static float m_MaxStateTime{ 10.f };
};

class ZakoDivingState final : public ZakoState
{
public:
	ZakoDivingState( StateMachine<ZakoState>* pParent );
	virtual void Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;
};

class ZakoReturningState final : public ZakoState
{
public:
	ZakoReturningState( StateMachine<ZakoState>* pParent );
	virtual void Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
} // namespace dae
#endif
