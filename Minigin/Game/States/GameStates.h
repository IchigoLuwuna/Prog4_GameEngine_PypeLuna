#ifndef GAMESTATES_H
#define GAMESTATES_H
#include <string>
#include <Helpers.h>
#include "Game/Classes/HiveMind.h"
#include "States/State.h"

namespace dae
{
class GameStartState : public State
{
public:
	GameStartState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

private:
	bool m_StartPressed{};
};

// After the Start Menu, before playing lvl 1
class GameEnterState : public State
{
public:
	GameEnterState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

private:
	static constexpr std::string m_StartSoundFile{ "start.wav" };
	static constexpr float m_StartSoundLength{ 6.361f };

	float m_StateTime{};
};

class GameStageTransitionState : public State
{
public:
	GameStageTransitionState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

	void SetNextStageNr( size_t nr );

private:
	static const std::string m_TransitionSoundFile;
	static constexpr float m_TransitionLength{ 1.5f };
	float m_StateTime{};
	size_t m_NextStageNr{};
};

class GameStageState : public State
{
public:
	GameStageState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

	void SetStageNr( size_t nr );

private:
	static constexpr uint32_t m_StageCount{ 3 };
	std::array<std::array<uint32_t, 3>, m_StageCount> m_StageEnemyCounts{ {
		{ 18, 14, 5 },
		{ 22, 18, 7 },
		{ 26, 22, 9 },
	} };

	HiveMind m_HiveMind{};
	Subscription m_Subscription;
	float m_TimeSinceLastCheck{};
	size_t m_StageNr{};
	bool m_PlayerRanOutOfLives{};

	void HandleEvent( Event& event );
};

class GameAllStagesClearState : public State
{
public:
	GameAllStagesClearState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

private:
	static constexpr float m_TransitionLength{ 3.f };
	float m_StateTime{};
};

class GameScoreboardState : public State
{
public:
	GameScoreboardState( StateMachine* pParent );

	virtual State* Update() override;
	virtual void Enter() override;
	virtual void Exit() override;

private:
	uint32_t m_PlayerScore{};
	bool m_StartPressed{};
};
} // namespace dae
#endif
