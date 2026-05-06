#include "HitboxRegistry.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include <glm/glm.hpp>

void dae::HitboxRegistry::DoCollisions()
{
	for ( auto& hitbox : m_RegisteredHitboxes )
	{
		for ( auto& targetHash : hitbox->GetTargets() )
		{
			if ( !m_RegisteredHurtboxes.contains( targetHash ) )
			{
				continue;
			}
			auto& hurtboxes{ m_RegisteredHurtboxes[targetHash] };
			for ( auto& hurtbox : hurtboxes )
			{
				if ( Collides( hitbox->m_Rectangle, hurtbox->m_Rectangle ) )
				{
					hitbox->OnCollision( hurtbox );
					hurtbox->OnCollision( hitbox );
				}
			}
		}
	}
}

void dae::HitboxRegistry::RegisterHitbox( Hitbox* pHitbox )
{
	m_RegisteredHitboxes.push_back( pHitbox );
}
void dae::HitboxRegistry::RemoveHitbox( const Hitbox* pHitbox )
{
	std::erase_if( m_RegisteredHitboxes, [&]( auto& pCurrentHitbox ) { return pHitbox == pCurrentHitbox; } );
}

void dae::HitboxRegistry::RegisterHurtbox( Hurtbox* pHurtbox, size_t targetHash )
{
	if ( !m_RegisteredHurtboxes.contains( targetHash ) )
	{
		m_RegisteredHurtboxes[targetHash] = {};
	}

	m_RegisteredHurtboxes[targetHash].push_back( pHurtbox );
}
void dae::HitboxRegistry::RemoveHurtbox( const Hurtbox* pHurtbox, size_t targetHash )
{
	std::erase_if( m_RegisteredHurtboxes[targetHash],
				   [&]( auto& pCurrentHurtbox ) { return pHurtbox == pCurrentHurtbox; } );
}

bool dae::HitboxRegistry::Collides( const glm::vec4& a, const glm::vec4& b )
{
	const float aRight{ a.x + a.z };
	const float bRight{ b.x + b.z };
	const float aBottom{ a.y + a.w };
	const float bBottom{ b.y + b.w };

	if ( aRight < b.x )
	{
		return false;
	}
	if ( bRight < a.x )
	{
		return false;
	}
	if ( aBottom < b.y )
	{
		return false;
	}
	if ( bBottom < a.y )
	{
		return false;
	}

	return true;
}
