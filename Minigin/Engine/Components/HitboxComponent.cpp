#include "HitboxComponent.h"
#include "TransformComponent.h"

dae::HitboxComponent::HitboxComponent( GameObject* pParent,
									   const glm::vec4& baseRect,
									   const std::vector<size_t>& targetHashes,
									   const std::function<void( GameObject*, Hurtbox* )> callback )
	: Component( pParent )
	, m_Hitbox( pParent, baseRect, targetHashes, callback )
	, m_BaseRect( baseRect )
{
	Update();
}

void dae::HitboxComponent::Update()
{
	auto transform{ GetParent()->GetComponent<TransformComponent>() };
	const glm::vec4 rect{
		transform->GetPosition().x + m_BaseRect.x,
		transform->GetPosition().y + m_BaseRect.y,
		m_BaseRect.z,
		m_BaseRect.w,
	};

	m_Hitbox.m_Rectangle = rect;
}
