#ifndef GOEISTATES_H
#define GOEISTATES_H
#include <States.h>
#include "Game/Components/BrainComponent.h"
#include <Maths.h>

namespace dae
{
// The bee guys
class GameObject;

class GoeiIdlingState final : public State
{
public:
	GoeiIdlingState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	float m_RemainingStateTime{};

	constexpr static float m_MinStateTime{ 1.f };
	constexpr static float m_MaxStateTime{ 20.f };
};

class GoeiDivingState final : public State
{
public:
	GoeiDivingState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_DivingTime{};
	float m_WiggleWidth{};
	float m_WiggleSpeed{};
	bool m_PlayerFound{};
};

class GoeiSecondDiveState final : public State
{
public:
	GoeiSecondDiveState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_DivingTime{};
	bool m_PlayerFound{};
};

class GoeiReturningState final : public State
{
public:
	GoeiReturningState( StateMachine* pParent );
	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	float m_ReturningTime{};
};
} // namespace dae
#endif
