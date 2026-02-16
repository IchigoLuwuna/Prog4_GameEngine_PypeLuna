#pragma once
#include <string>
#include <memory>
#include <SDL3/SDL_pixels.h>
#include "GameObject.h"
#include "Transform.h"

namespace dae
{
class Font;
class Texture2D;
class TextObject final : public GameObject
{
public:
	TextObject() = default;

	void Update() override;
	void Render() const override;

	void SetText( const std::string& text );
	void SetPosition( float x, float y );
	void SetColor( const SDL_Color& color );

	TextObject( const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 } );
	virtual ~TextObject() = default;
	TextObject( const TextObject& other ) = delete;
	TextObject( TextObject&& other );
	TextObject& operator=( const TextObject& other ) = delete;
	TextObject& operator=( TextObject&& other );

private:
	bool m_NeedsUpdate{};
	std::string m_Text{};
	SDL_Color m_Color{ 255, 255, 255, 255 };
	Transform m_Transform{};
	std::shared_ptr<Font> m_Font{};
	std::shared_ptr<Texture2D> m_TextTexture{};
};
} // namespace dae
