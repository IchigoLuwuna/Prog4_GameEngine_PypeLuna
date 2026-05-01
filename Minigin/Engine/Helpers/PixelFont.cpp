#include "PixelFont.h"
#include "Rendering.h"
#include <iostream>
#include "Core/ResourceManager.h"

dae::PixelFont::PixelFont( const std::string& path, const std::string& mapping, const glm::vec2& dimensions )
	: m_TypeFace( ResourceManager::GetInstance().LoadTexture( path ) )
	, m_Mapping( mapping )
	, m_CharDimensions( dimensions )
{
}

dae::PixelFont::PixelFont( const std::string& path, std::string&& mapping, const glm::vec2& dimensions )
	: m_TypeFace( path )
	, m_Mapping( std::move( mapping ) )
	, m_CharDimensions( dimensions )
{
}

void dae::PixelFont::Render( const std::string& text, const glm::vec2& position ) const
{
	glm::vec2 printHead{ position };

	// For every character
	for ( auto character : text )
	{
		// Contains either 1101 1111 or 1111 1111
		const char& ignoreMask{ static_cast<char>( static_cast<char>( m_IgnoreCapital << 5 ) ^ 0xFF ) };

		if ( character == '\n' )
		{
			printHead.x = position.x;
			printHead.y += m_CharDimensions.y;
			continue;
		}
		if ( character == ' ' )
		{
			printHead.x += m_CharDimensions.x;
			continue;
		}

		bool mappingFound{};
		size_t mappingIdx{};
		for ( auto& mappedChar : m_Mapping )
		{
			if ( ( mappedChar & ignoreMask ) == ( character & ignoreMask ) )
			{
				mappingFound = true;
				break;
			}
			++mappingIdx;
		}
		if ( !mappingFound )
		{
#ifndef NDEBUG
			std::cout << "Tried to print unmapped character '" << character << "'\n";
#endif
			printHead.x += m_CharDimensions.x;
			continue;
		}

		SDL_FRect srcRect{ mappingIdx * m_CharDimensions.x, 0.f, m_CharDimensions.x, m_CharDimensions.y };
		SDL_FRect dstRect{ printHead.x, printHead.y, m_CharDimensions.x, m_CharDimensions.y };
		Renderer::GetInstance().RenderTexture( *m_TypeFace, srcRect, dstRect );
		printHead.x += m_CharDimensions.x;
	}
}

void dae::PixelFont::SetIgnoreCapital( bool ignore )
{
	m_IgnoreCapital = ignore;
}
