#include "TTFTextComponent.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "Rendering/Renderer.h"
#include <stdexcept>
#include <Components.h>
#include <Rendering.h>

dae::TTFTextComponent::TTFTextComponent( GameObject* pParent,
										 const std::string& text,
										 dae::ReferencePtr<Font> font,
										 const SDL_Color& color )
	: TextComponent( pParent )
	, m_Text( text )
	, m_Color( color )
	, m_Font( font )
{
	Update();
}

void dae::TTFTextComponent::Update()
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

void dae::TTFTextComponent::Render() const
{
	// Text gets rendered at an internal 4x resolution

	glm::vec2 renderScale{};
	SDL_GetRenderScale( Renderer::GetInstance().GetSDLRenderer(), &renderScale.x, &renderScale.y );
	SDL_SetRenderScale( Renderer::GetInstance().GetSDLRenderer(), renderScale.x / 4.f, renderScale.y / 4.f );
	auto position{ GetParent()->GetComponent<TransformComponent>() };
	Renderer::GetInstance().RenderTexture( *m_TextTexture, position->GetPosition().x, position->GetPosition().y );
	SDL_SetRenderScale( Renderer::GetInstance().GetSDLRenderer(), renderScale.x, renderScale.y );
}

void dae::TTFTextComponent::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TTFTextComponent::SetColor( const SDL_Color& color )
{
	m_Color = color;
	m_NeedsUpdate = true;
}
