#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include <cstdint>
#include <Patterns.h>

namespace dae
{
class HealthComponent : public Component
{
public:
	HealthComponent( GameObject* pGameObject, uint32_t health, uint32_t maxHealth = 0, float invincibleTime = 0.f );
	virtual ~HealthComponent() = default;

	virtual void Update() override;

	void Heal( uint32_t healing );
	void Damage( uint32_t damage );
	void IncreaseMax( uint32_t increase );
	void DecreaseMax( uint32_t decrease );
	void SetInvincibleTime( uint32_t newInvincibleTime );

	uint32_t GetHealth() const;
	uint32_t GetMaxHealth() const;
	bool IsDead() const;

	template <typename T>
	void RegisterObserver( ReferencePtr<T> pObserver )
	{
		m_Messenger.RegisterObserver( pObserver );
	}
	void RemoveObserver( Observer* pObserver );

private:
	uint32_t m_Health{};
	uint32_t m_MaxHealth{};
	float m_InvincibleTime{};

	Messenger<HealthComponent> m_Messenger;
};
} // namespace dae
#endif
