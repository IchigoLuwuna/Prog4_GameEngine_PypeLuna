#include "SpriteSheetComponent.h"
#include <Components.h>

dae::SpriteSheetComponent::SpriteSheetComponent( GameObject* pParent,
												 const std::string& path,
												 const SpriteSheet::SpriteSheetInfo& info )
	: RenderComponent( pParent )
	, m_Sheet( path, info )
{
}

void dae::SpriteSheetComponent::Render() const
{
	m_Sheet.Render( GetParent()->GetComponent<dae::TransformComponent>()->GetPosition() );
}

void dae::SpriteSheetComponent::SetIndex( uint32_t index )
{
	m_Sheet.SetIndex( index );
}
void dae::SpriteSheetComponent::SetIndex( uint32_t column, uint32_t row )
{
	m_Sheet.SetIndex( column, row );
}
