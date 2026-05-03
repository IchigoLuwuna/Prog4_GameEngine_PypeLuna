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

glm::vec2 dae::PixelTextComponent::GetSize() const
{
	if ( !m_SizeDirty )
	{
		return m_Size;
	}

	int columns{};
	int rows{};
	int charsInLine{};
	for ( auto& character : m_Text )
	{
		if ( character == '\n' )
		{
			charsInLine = 0;
			++rows;
			continue;
		}

		++charsInLine;
		if ( charsInLine > columns )
		{
			columns = charsInLine;
		}
	}
	glm::vec2 size{ columns * m_Font.GetCharSize().x, rows * m_Font.GetCharSize().y };
	m_Size = size;
	m_SizeDirty = false;
	return m_Size;
}

void dae::PixelTextComponent::SetText( const std::string& text )
{
	m_Text = text;
	m_SizeDirty = true;
}

dae::PixelTextComponent& dae::PixelTextComponent::SetIgnore( bool ignore )
{
	m_Font.SetIgnoreCapital( ignore );
	return *this;
}
