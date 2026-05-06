#include "ProjectileAmmoComponent.h"
#include <Helpers.h>

dae::ProjectileAmmoComponent::ProjectileAmmoComponent( GameObject* pParent, uint32_t ammo )
	: Component( pParent )
	, m_Count( ammo )
{
}

bool dae::ProjectileAmmoComponent::HasAmmo() const
{
	return m_Count;
}

void dae::ProjectileAmmoComponent::AddAmmo()
{
	++m_Count;
}

void dae::ProjectileAmmoComponent::DecreaseAmmo()
{
	if ( m_Count == 0 )
	{
		return;
	}

	--m_Count;
}

void dae::ProjectileAmmoComponent::Notify( size_t eventHash, ProjectileComponent* )
{
	switch ( eventHash )
	{
	case "e_ProjectileHit"_hash:
	case "e_ProjectileTimeout"_hash: {
		AddAmmo();
		break;
	}
	default: {
		return;
	}
	}
}
