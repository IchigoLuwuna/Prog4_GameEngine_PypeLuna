#ifndef AMMOLIMITERCOMPONENT_H
#define AMMOLIMITERCOMPONENT_H
#include <Patterns.h>

namespace dae
{
class ProjectileComponent;
class ProjectileAmmoComponent : public Component, public Observer
{
public:
	ProjectileAmmoComponent( GameObject* pParent, uint32_t ammo );
	virtual ~ProjectileAmmoComponent() = default;

	bool HasAmmo() const;
	void AddAmmo();
	void DecreaseAmmo();

	virtual void Update() override { };
	virtual void Notify( size_t eventHash, void* pSubject ) override;

private:
	uint32_t m_Count{};
};
} // namespace dae
#endif
