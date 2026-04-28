#ifndef DAMAGECOMMAND_H
#define DAMAGECOMMAND_H
#include <cstdint>
#include "Engine/Helpers/Subscription.h"
#include "Engine/Memory/ReferencePtr.h"
#include "Engine/Patterns/Command.h"

namespace dae
{
class HealthComponent;
class DamageCommand : public Command
{
public:
	DamageCommand( const ReferencePtr<HealthComponent>& pBoundHealth, uint32_t damage );
	virtual ~DamageCommand() = default;

	virtual void Execute() override;

private:
	ReferencePtr<HealthComponent> m_pBoundHealth{};
	uint32_t m_Damage{};

	void HandleEvent( Event& event );
};
} // namespace dae
#endif
