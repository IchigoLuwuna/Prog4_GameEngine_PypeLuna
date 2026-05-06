#include "HurtboxComponent.h"
#include "TransformComponent.h"

dae::HurtboxComponent::HurtboxComponent( GameObject* pParent,
										 const glm::vec4& baseRect,
										 size_t targetHash,
										 const std::function<void( GameObject*, Hitbox* )> callback )
	: Component( pParent )
	, m_Hurtbox( pParent, baseRect, targetHash, callback )
	, m_BaseRect( baseRect )
{
}

void dae::HurtboxComponent::Update()
{
	auto transform{ GetParent()->GetComponent<TransformComponent>() };
	const glm::vec4 rect{
		transform->GetPosition().x + m_BaseRect.x,
		transform->GetPosition().y + m_BaseRect.y,
		m_BaseRect.z,
		m_BaseRect.w,
	};

	m_Hurtbox.m_Rectangle = rect;
}
