#include "TextAllignmentComponent.h"
#include <Components.h>

dae::TextAllignmentComponent::TextAllignmentComponent( GameObject* pParent, glm::vec2 origin, Allignment allignment )
	: Component( pParent )
	, m_TextRef( GetParent()->GetComponent<dae::TextComponent>() )
	, m_Origin( origin )
	, m_Allignment( allignment )
{
	assert( m_TextRef.Get() && "Could not find text component to allign" );
}

void dae::TextAllignmentComponent::Update()
{
	if ( !m_TextRef.Validate() )
	{
		return;
	}

	switch ( m_Allignment )
	{
	case Allignment::topLeft: {
		// Dummy, no allignment needed
		break;
	}
	case Allignment::topRight: {
		const glm::vec2 textSize{ m_TextRef->GetSize() };
		const glm::vec2 moveToPos{ m_Origin.x - textSize.x, m_Origin.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	case Allignment::bottomLeft: {
		const glm::vec2 textSize{ m_TextRef->GetSize() };
		const glm::vec2 moveToPos{ m_Origin.x, m_Origin.y - textSize.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	case Allignment::bottomRight: {
		const glm::vec2 textSize{ m_TextRef->GetSize() };
		const glm::vec2 moveToPos{ m_Origin.x - textSize.x, m_Origin.y - textSize.y };
		GetParent()->GetComponent<dae::TransformComponent>()->MoveTo( moveToPos );
		break;
	}
	}
}
