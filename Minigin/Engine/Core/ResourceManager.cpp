#include <iostream>
#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Helpers/Texture2D.h"
#include "Helpers/Font.h"

namespace fs = std::filesystem;

void dae::ResourceManager::Init( const std::filesystem::path& dataPath )
{
	m_dataPath = dataPath;

	if ( !TTF_Init() )
	{
		throw std::runtime_error( std::string( "Failed to load support for fonts: " ) + SDL_GetError() );
	}
}

void dae::ResourceManager::Destroy()
{
	m_LoadedFonts.clear();
	m_LoadedTextures.clear();
}

dae::ReferencePtr<dae::Texture2D> dae::ResourceManager::LoadTexture( const std::string& file )
{
	const auto fullPath = m_dataPath / file;
	const auto filename = fs::path( fullPath ).filename().string();
	if ( m_LoadedTextures.find( filename ) == m_LoadedTextures.end() )
	{
		SafePtr<Texture2D> texture{ std::make_unique<Texture2D>( fullPath.string() ) };
		m_LoadedTextures.insert( std::pair( filename, std::move( texture ) ) );
	}
	return ReferencePtr( m_LoadedTextures.at( filename ) );
}

dae::ReferencePtr<dae::Font> dae::ResourceManager::LoadFont( const std::string& file, uint8_t size )
{
	const auto fullPath = m_dataPath / file;
	const auto filename = fs::path( fullPath ).filename().string();
	const auto key = std::pair<std::string, uint8_t>( filename, size );
	if ( m_LoadedFonts.find( key ) == m_LoadedFonts.end() )
	{
		SafePtr<Font> font{ std::make_unique<Font>( fullPath.string(), size ) };
		m_LoadedFonts.insert( std::pair( key, std::move( font ) ) );
	}
	return ReferencePtr( m_LoadedFonts.at( key ) );
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for ( auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end(); )
	{
		if ( it->second.GetRefCount() == 0 )
		{
			it = m_LoadedTextures.erase( it );
		}
		else
		{
			++it;
		}
	}

	for ( auto it = m_LoadedFonts.begin(); it != m_LoadedFonts.end(); )
	{
		if ( it->second.GetRefCount() == 0 )
		{
			it = m_LoadedFonts.erase( it );
		}
		else
		{
			++it;
		}
	}
}
