#ifndef TTFTEXTCOMPONENT_H
#define TTFTEXTCOMPONENT_H
#include <SDL3/SDL_pixels.h>
#include <Patterns.h>
#include <Helpers.h>
#include "TextComponent.h"

namespace dae
{
class TTFTextComponent : public TextComponent
{
public:
	TTFTextComponent( GameObject* pParent,
					  const std::string& text,
					  ReferencePtr<Font> font,
					  const SDL_Color& color = { 255, 255, 255, 255 } );
	virtual ~TTFTextComponent() = default;

	virtual void Update() override;
	virtual void Render() const override;

	virtual glm::vec2 GetSize() const override;

	virtual void SetText( const std::string& text ) override;
	virtual void SetColor( const SDL_Color& color ) override;

private:
	bool m_NeedsUpdate{ true };
	std::string m_Text{};
	SDL_Color m_Color{ 255, 255, 255, 255 };
	ReferencePtr<Font> m_Font{};
	std::unique_ptr<Texture2D> m_TextTexture{};
};
} // namespace dae
#endif
