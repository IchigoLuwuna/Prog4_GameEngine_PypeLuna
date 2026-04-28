#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include <cstdint>
#include "Engine/Patterns/Component.h"
#include "Engine/Patterns/Observer.h"

namespace dae
{
class HealthComponent : public Component
{
public:
	HealthComponent( GameObject* pGameObject, uint32_t health, uint32_t maxHealth = 0 );
	virtual ~HealthComponent() = default;

	virtual void Update() override
	{
	}

	void Heal( uint32_t healing );
	void Damage( uint32_t damage );
	void IncreaseMax( uint32_t increase );
	void DecreaseMax( uint32_t decrease );

	uint32_t GetHealth() const;
	uint32_t GetMaxHealth() const;
	bool IsDead() const;

	void RegisterObserver( Observer<HealthComponent>* pObserver );
	void RemoveObserver( Observer<HealthComponent>* pObserver );

private:
	uint32_t m_Health{};
	uint32_t m_MaxHealth{};

	Messenger<HealthComponent> m_Subject;
};
} // namespace dae
#endif
