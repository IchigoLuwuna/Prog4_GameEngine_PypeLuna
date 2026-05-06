#ifndef SPRITESHEET_H
#define SPRITESHEET_H
#include "Helpers/Texture2D.h"
#include "Memory/ReferencePtr.h"

namespace dae
{
// Spritesheet must be uniform
class SpriteSheet
{
public:
	struct SpriteSheetInfo
	{
		uint32_t columns{};
		uint32_t rows{};
	};

	SpriteSheet( const std::string& path, const SpriteSheetInfo& info );

	void Render( const glm::vec2 pos ) const;

	void SetIndex( uint32_t index );
	void SetIndex( uint32_t column, uint32_t row );

private:
	ReferencePtr<Texture2D> m_Sheet{};
	SpriteSheetInfo m_Info{};
	uint32_t m_Index{};
};
} // namespace dae
#endif
