#pragma once
#include <filesystem>
#include <string>
#include "Memory/SafePtr.h"
#include "Memory/ReferencePtr.h"
#include <map>
#include <unordered_map>
#include "Patterns/Singleton.h"
#include "Helpers/Font.h"
#include "Helpers/Texture2D.h"

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

	const std::filesystem::path& GetDataPath() const;

	void UnloadUnusedResources();

private:
	friend class Singleton<ResourceManager>;
	ResourceManager() = default;
	std::filesystem::path m_dataPath;

	std::unordered_map<std::string, SafePtr<Texture2D>> m_LoadedTextures;
	std::map<std::pair<std::string, uint8_t>, SafePtr<Font>> m_LoadedFonts;
};
} // namespace dae
