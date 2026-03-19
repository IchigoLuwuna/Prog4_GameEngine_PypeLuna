#ifndef INCREASESCORECOMMAND_H
#define INCREASESCORECOMMAND_H
#include "Engine/Patterns/Command.h"
#include "Engine/Helpers/Subscription.h"
#include <cstdint>

namespace dae
{
class ScoreComponent;
class IncreaseScoreCommand : public Command
{
public:
	IncreaseScoreCommand( ScoreComponent* pBoundScore, uint32_t increase );
	virtual ~IncreaseScoreCommand() = default;

	virtual void Execute() override;

private:
	ScoreComponent* m_pBoundScore{};
	uint32_t m_Increase{};
	Subscription m_Listener;

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
