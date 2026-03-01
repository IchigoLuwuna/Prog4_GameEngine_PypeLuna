#include "TextComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"

dae::TextComponent::TextComponent( GameObject* pParent,
								   const std::string& text,
								   std::shared_ptr<Font> font,
								   const SDL_Color& color )
	: RenderComponent( pParent )
	, m_Text( text )
	, m_Color( color )
	, m_Font( font )
{
	Update();
}

void dae::TextComponent::Update()
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
		m_TextTexture = std::make_unique<Texture2D>( texture );
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	auto position{ GetParent()->GetComponent<TransformComponent>() };
	Renderer::GetInstance().RenderTexture( *m_TextTexture, position->GetPosition().x, position->GetPosition().y );
}

void dae::TextComponent::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor( const SDL_Color& color )
{
	m_Color = color;
	m_NeedsUpdate = true;
}
