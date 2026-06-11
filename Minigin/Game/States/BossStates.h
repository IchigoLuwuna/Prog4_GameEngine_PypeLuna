#ifndef BOSSSTATES_H
#define BOSSSTATES_H
#include "Game/Components/BrainComponent.h"
#include "Math/Bezier.h"
#include "States/State.h"

namespace dae
{
class BossIdlingState : public State
{
public:
	BossIdlingState( StateMachine* pParent )
		: State( pParent )
	{
	}

	virtual State* Update() override
	{
		return nullptr;
	}
	virtual void Enter() override
	{
	}
	virtual void Exit() override
	{
	}

private:
};

class BossReturningState : public State
{
public:
	BossReturningState( StateMachine* pParent );
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
