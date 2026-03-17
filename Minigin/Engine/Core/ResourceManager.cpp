#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Helpers/Texture2D.h"
#include "Engine/Helpers/Font.h"

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

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture( const std::string& file )
{
	const auto fullPath = m_dataPath / file;
	const auto filename = fs::path( fullPath ).filename().string();
	if ( m_LoadedTextures.find( filename ) == m_LoadedTextures.end() )
		m_LoadedTextures.insert( std::pair( filename, std::make_shared<Texture2D>( fullPath.string() ) ) );
	return m_LoadedTextures.at( filename );
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont( const std::string& file, uint8_t size )
{
	const auto fullPath = m_dataPath / file;
	const auto filename = fs::path( fullPath ).filename().string();
	const auto key = std::pair<std::string, uint8_t>( filename, size );
	if ( m_LoadedFonts.find( key ) == m_LoadedFonts.end() )
		m_LoadedFonts.insert( std::pair( key, std::make_shared<Font>( fullPath.string(), size ) ) );
	return m_LoadedFonts.at( key );
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for ( auto it = m_LoadedTextures.begin(); it != m_LoadedTextures.end(); )
	{
		if ( it->second.use_count() == 1 )
			it = m_LoadedTextures.erase( it );
		else
			++it;
	}

	for ( auto it = m_LoadedFonts.begin(); it != m_LoadedFonts.end(); )
	{
		if ( it->second.use_count() == 1 )
			it = m_LoadedFonts.erase( it );
		else
			++it;
	}
}
