#ifndef PIXELFONT_H
#define PIXELFONT_H
#include "Helpers/Texture2D.h"
#include "Memory/ReferencePtr.h"

namespace dae
{
class PixelFont
{
public:
	PixelFont( const std::string& path, const std::string& mapping, const glm::vec2& dimensions );
	PixelFont( const std::string& path, std::string&& mapping, const glm::vec2& dimensions );

	void Render( const std::string& text, const glm::vec2& position ) const;
	void SetIgnoreCapital( bool ignore );

	glm::vec2 GetCharSize() const;

private:
	ReferencePtr<Texture2D> m_TypeFace;
	std::string m_Mapping{};
	glm::vec2 m_CharDimensions{};
	bool m_IgnoreCapital{};
#ifndef NDEBUG
	mutable bool m_HasComplained{};
#endif
};
} // namespace dae
#endif
