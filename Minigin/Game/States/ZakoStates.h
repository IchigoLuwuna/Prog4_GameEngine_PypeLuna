#ifndef ZAKOSTATES_H
#define ZAKOSTATES_H
#include <States.h>
#include "Game/Components/BrainComponent.h"
#include "Math/Bezier.h"

namespace dae
{
// The bee guys
class GameObject;

class ZakoIdlingState final : public State
{
public:
	ZakoIdlingState( StateMachine* pParent );
	virtual State* Update() override;

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
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_DivingTime{};
	bool m_PlayerFound{};
};

class ZakoSecondDiveState final : public State
{
public:
	ZakoSecondDiveState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_DivingTime{};
	bool m_PlayerFound{};
};

class ZakoReturningState final : public State
{
public:
	ZakoReturningState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_ReturningTime{};
};
} // namespace dae
#endif
