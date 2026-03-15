#ifndef DAMAGECOMMAND_H
#define DAMAGECOMMAND_H
#include <cstdint>
#include "Engine/Patterns/Command.h"

namespace dae
{
class HealthComponent;
class DamageCommand : public Command
{
public:
	DamageCommand( HealthComponent* pBoundHealth, uint32_t damage );
	virtual ~DamageCommand() = default;

	virtual void Execute() override;

private:
	HealthComponent* m_pBoundHealth{};
	uint32_t m_Damage{};
};
} // namespace dae
#endif
