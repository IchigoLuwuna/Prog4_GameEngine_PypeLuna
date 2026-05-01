#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H
#include <SDL3/SDL_pixels.h>
#include <Patterns.h>

// Allows for both vector-based and pixel-based text rendering solutions using the same interface
namespace dae
{
class TextComponent : public RenderComponent
{
public:
	TextComponent( GameObject* pParent )
		: RenderComponent( pParent )
	{
	}
	virtual ~TextComponent() = default;

	virtual void SetText( const std::string& text ) = 0;
	virtual void SetColor( const SDL_Color& color ) = 0;
};
} // namespace dae
#endif
