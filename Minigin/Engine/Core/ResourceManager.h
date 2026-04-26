#pragma once
#include <filesystem>
#include <string>
#include "Engine/Memory/SafePtr.h"
#include "Engine/Memory/ReferencePtr.h"
#include <map>
#include <unordered_map>
#include "Engine/Patterns/Singleton.h"
#include "Engine/Helpers/Font.h"
#include "Engine/Helpers/Texture2D.h"

namespace dae
{
class Texture2D;
// class Font;
class ResourceManager final : public Singleton<ResourceManager>
{
public:
	void Init( const std::filesystem::path& data );
	void Destroy();
	ReferencePtr<Texture2D> LoadTexture( const std::string& file );
	ReferencePtr<Font> LoadFont( const std::string& file, uint8_t size );

	void UnloadUnusedResources();

private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
	std::filesystem::path m_dataPath;

	std::unordered_map<std::string, SafePtr<Texture2D>> m_LoadedTextures;
	std::map<std::pair<std::string, uint8_t>, SafePtr<Font>> m_LoadedFonts;
};
} // namespace dae
