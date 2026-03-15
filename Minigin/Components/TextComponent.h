#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H
#include <memory>
#include <SDL3/SDL_pixels.h>
#include "Engine/Helpers/Font.h"
#include "Engine/Helpers/Texture2D.h"
#include "Engine/Patterns/RenderComponent.h"

namespace dae
{
class TextComponent : public RenderComponent
{
public:
	TextComponent( GameObject* pParent,
				   const std::string& text,
				   std::shared_ptr<Font> font,
				   const SDL_Color& color = { 255, 255, 255, 255 } );
	virtual ~TextComponent() = default;

	virtual void Update() override;
	virtual void Render() const override;

	void SetText( const std::string& text );
	void SetColor( const SDL_Color& color );

private:
	bool m_NeedsUpdate{ true };
	std::string m_Text{};
	SDL_Color m_Color{ 255, 255, 255, 255 };
	std::shared_ptr<Font> m_Font{};
	std::unique_ptr<Texture2D> m_TextTexture{};
};
} // namespace dae

#endif
