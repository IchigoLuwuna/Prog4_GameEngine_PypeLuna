#include "SpriteSheet.h"
#include "Core.h"
#include "Rendering.h"

dae::SpriteSheet::SpriteSheet( const std::string& path, const SpriteSheetInfo& info )
	: m_Sheet( ResourceManager::GetInstance().LoadTexture( path ) )
	, m_Info( info )
{
}

void dae::SpriteSheet::Render( const glm::vec2 pos ) const
{
	const glm::vec2 spriteDimensions{
		m_Sheet->GetSize().x / m_Info.columns,
		m_Sheet->GetSize().y / m_Info.rows,
	};

	const SDL_FRect src{
		( m_Index % m_Info.columns ) * spriteDimensions.x,
		( m_Index / m_Info.columns ) * spriteDimensions.y,
		spriteDimensions.x,
		spriteDimensions.y,
	};
	const SDL_FRect dst{
		pos.x,
		pos.y,
		spriteDimensions.x,
		spriteDimensions.y,
	};

	Renderer::GetInstance().RenderTexture( *m_Sheet, src, dst );
}

void dae::SpriteSheet::SetIndex( uint32_t index )
{
	assert( index < m_Info.columns * m_Info.rows && "Index is larger than sheet dimensions" );
	m_Index = index;
}
void dae::SpriteSheet::SetIndex( uint32_t column, uint32_t row )
{
	const uint32_t newIdx{ column + m_Info.columns * row };
	assert( newIdx < m_Info.columns * m_Info.rows && "Index is larger than sheet dimensions" );
	m_Index = newIdx;
}
