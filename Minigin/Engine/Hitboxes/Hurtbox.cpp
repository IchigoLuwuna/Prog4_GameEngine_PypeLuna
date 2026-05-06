#include "Hurtbox.h"
#include "Hitboxes/HitboxRegistry.h"

dae::Hurtbox::Hurtbox( GameObject* pParent,
					   const glm::vec4& rect,
					   size_t targetHash,
					   const std::function<void( GameObject*, Hitbox* )> callback )
	: m_Rectangle( rect )
	, m_pParent( pParent )
	, m_TargetHash( targetHash )
	, m_Callback( callback )
{
	HitboxRegistry::GetInstance().RegisterHurtbox( this, targetHash );
}
dae::Hurtbox::~Hurtbox()
{
	HitboxRegistry::GetInstance().RemoveHurtbox( this, m_TargetHash );
}

dae::GameObject* dae::Hurtbox::GetParent() const
{
	return m_pParent;
}

void dae::Hurtbox::OnCollision( Hitbox* pOther )
{
	if ( m_Callback )
	{
		m_Callback( m_pParent, pOther );
	}
}
