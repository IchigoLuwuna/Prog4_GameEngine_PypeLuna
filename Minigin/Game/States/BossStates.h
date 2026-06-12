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
	BossIdlingState( StateMachine* pParent );

	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	float m_StateTime{};
};

class BossDivingState : public State
{
public:
	BossDivingState( StateMachine* pParent );

	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	math::Bezier4 m_BezierPath{};
	float m_DivingTime{};
	bool m_PlayerFound{};
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

	float m_ReturningTime{};
};

class BossTractorBeamState : public State
{
public:
	BossTractorBeamState( StateMachine* pParent );

	virtual State* Update() override;

	virtual void Enter() override;
	virtual void Exit() override;

	void MarkTractorCaptured();

private:
	ReferencePtr<BrainComponent> m_BrainRef{};

	float m_StateTime{};
	bool m_TractorCaptured{};

	std::unique_ptr<GameObject> MakeTractorBeam();
};
} // namespace dae
#endif
