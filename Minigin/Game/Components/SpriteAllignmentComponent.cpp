#include "SpriteAllignmentComponent.h"
#include "Components/TransformComponent.h"
#include "Game/Components/SpriteSheetComponent.h"

dae::SpriteAllignmentComponent::SpriteAllignmentComponent( GameObject* pParent,
														   glm::vec2 origin,
														   Allignment allignment )
	: Component( pParent )
	, m_SpritesheetRef( GetParent()->GetComponent<dae::SpriteSheetComponent>() )
	, m_Origin( origin )
	, m_Allignment( allignment )
{
}

void dae::SpriteAllignmentComponent::Update()
{
	if ( !m_SpritesheetRef.Validate() )
	{
		return;
	}

	switch ( m_Allignment )
	{
	case Allignment::center: {
		const glm::vec2 size{ m_SpritesheetRef->GetSpriteSize() };
		const glm::vec2 offset{ -size / 2.f };
		const glm::vec2 moveToPos{ m_Origin + offset };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	case Allignment::centerTop: {
		const glm::vec2 size{ m_SpritesheetRef->GetSpriteSize() };
		const glm::vec2 offset{ -size.x / 2.f, size.y };
		const glm::vec2 moveToPos{ m_Origin + offset };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
		break;
	}
	case Allignment::topLeft: {
		// No allignment needed
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( m_Origin );
		break;
	}
	case Allignment::topRight: {
		const glm::vec2 size{ m_SpritesheetRef->GetSpriteSize() };
		const glm::vec2 moveToPos{ m_Origin.x - size.x, m_Origin.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	case Allignment::bottomLeft: {
		const glm::vec2 textSize{ m_SpritesheetRef->GetSpriteSize() };
		const glm::vec2 moveToPos{ m_Origin.x, m_Origin.y - textSize.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	case Allignment::bottomRight: {
		const glm::vec2 textSize{ m_SpritesheetRef->GetSpriteSize() };
		const glm::vec2 moveToPos{ m_Origin.x - textSize.x, m_Origin.y - textSize.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	}
}
