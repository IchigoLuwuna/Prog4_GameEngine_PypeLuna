#include "PixelTextComponent.h"
#include "Components/TransformComponent.h"

dae::PixelTextComponent::PixelTextComponent( GameObject* pParent,
											 const std::string& path,
											 const std::string& mapping,
											 const glm::vec2& dimensions )
	: TextComponent( pParent )
	, m_Font( path, mapping, dimensions )
{
}

dae::PixelTextComponent::PixelTextComponent( GameObject* pParent,
											 const std::string& path,
											 std::string&& mapping,
											 const glm::vec2& dimensions )
	: TextComponent( pParent )
	, m_Font( path, mapping, dimensions )
{
}

void dae::PixelTextComponent::Render() const
{
	m_Font.Render( m_Text, GetParent()->GetComponent<TransformComponent>()->GetPosition() );
}

void dae::PixelTextComponent::SetText( const std::string& text )
{
	m_Text = text;
}

dae::PixelTextComponent& dae::PixelTextComponent::SetIgnore( bool ignore )
{
	m_Font.SetIgnoreCapital( ignore );
	return *this;
}
