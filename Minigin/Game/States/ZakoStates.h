#ifndef ZAKOSTATES_H
#define ZAKOSTATES_H
#include <States.h>

namespace dae
{
// The bee guys
class GameObject;

class ZakoIdlingState final : public State
{
public:
	ZakoIdlingState( StateMachine* pParent );
	virtual State* Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	float m_RemainingStateTime{};

	constexpr static float m_MinStateTime{ 1.f };
	constexpr static float m_MaxStateTime{ 10.f };
};

class ZakoDivingState final : public State
{
public:
	ZakoDivingState( StateMachine* pParent );
	virtual State* Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	GameObject* m_pParent{};
	float m_StartingX{};
	bool m_StartingXSet{};
	float m_DivingTime{};
};

class ZakoReturningState final : public State
{
public:
	ZakoReturningState( StateMachine* pParent );
	virtual State* Update( GameObject* pObject ) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
} // namespace dae
#endif
