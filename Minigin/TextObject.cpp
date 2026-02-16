#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

using namespace dae;

TextObject::TextObject( const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color )
	: m_NeedsUpdate( true )
	, m_Text( text )
	, m_Color( color )
	, m_Font( std::move( font ) )
	, m_TextTexture( nullptr )
{
}

TextObject::TextObject( TextObject&& other )
{
	m_NeedsUpdate = other.m_NeedsUpdate;
	m_Text = std::move( other.m_Text );
	m_Color = other.m_Color;
	m_Transform = other.m_Transform;
	m_Font = std::move( other.m_Font );
	m_TextTexture = std::move( other.m_TextTexture );
}

TextObject& TextObject::operator=( TextObject&& other )
{
	if ( this == &other )
	{
		return *this;
	}

	m_NeedsUpdate = other.m_NeedsUpdate;
	m_Text = std::move( other.m_Text );
	m_Color = other.m_Color;
	m_Transform = other.m_Transform;
	m_Font = std::move( other.m_Font );
	m_TextTexture = std::move( other.m_TextTexture );

	return *this;
}

void TextObject::Update()
{
	if ( m_NeedsUpdate )
	{
		const auto surf = TTF_RenderText_Blended( m_Font->GetFont(), m_Text.c_str(), m_Text.length(), m_Color );
		if ( surf == nullptr )
		{
			throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError() );
		}
		auto texture = SDL_CreateTextureFromSurface( Renderer::GetInstance().GetSDLRenderer(), surf );
		if ( texture == nullptr )
		{
			throw std::runtime_error( std::string( "Create text texture from surface failed: " ) + SDL_GetError() );
		}
		SDL_DestroySurface( surf );
		m_TextTexture = std::make_shared<Texture2D>( texture );
		m_NeedsUpdate = false;
	}
}

void TextObject::Render() const
{
	if ( m_TextTexture != nullptr )
	{
		const auto& pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture( *m_TextTexture, pos.x, pos.y );
	}
}

void TextObject::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void TextObject::SetPosition( const float x, const float y )
{
	m_Transform.SetPosition( x, y );
}

void TextObject::SetColor( const SDL_Color& color )
{
	m_Color = color;
	m_NeedsUpdate = true;
}
