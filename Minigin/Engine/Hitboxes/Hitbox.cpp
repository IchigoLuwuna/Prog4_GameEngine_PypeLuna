#include "Hitbox.h"
#include "HitboxRegistry.h"

dae::Hitbox::Hitbox( GameObject* pParent,
					 const glm::vec4 rect,
					 const std::vector<size_t>& targetHashes,
					 const std::function<void( GameObject*, Hurtbox* )>& callback )
	: m_Rectangle( rect )
	, m_pParent( pParent )
	, m_TargetHashes( targetHashes )
	, m_Callback( callback )
{
	HitboxRegistry::GetInstance().RegisterHitbox( this );
}

dae::Hitbox::Hitbox( const glm::vec4 rect,
					 std::vector<size_t>&& targetHashes,
					 const std::function<void( GameObject*, Hurtbox* )>& callback )
	: m_Rectangle( rect )
	, m_TargetHashes( std::move( targetHashes ) )
	, m_Callback( callback )
{
	HitboxRegistry::GetInstance().RegisterHitbox( this );
}

dae::Hitbox::~Hitbox()
{
	HitboxRegistry::GetInstance().RemoveHitbox( this );
}

const std::vector<size_t>& dae::Hitbox::GetTargets() const
{
	return m_TargetHashes;
}

dae::GameObject* dae::Hitbox::GetParent() const
{
	return m_pParent;
}

void dae::Hitbox::OnCollision( Hurtbox* pOther )
{
	if ( m_Callback )
	{
		m_Callback( m_pParent, pOther );
	}
}
