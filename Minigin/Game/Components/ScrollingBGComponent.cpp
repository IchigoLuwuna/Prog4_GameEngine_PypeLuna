#include "ScrollingBGComponent.h"
#include <Rendering.h>
#include <SDL3/SDL_oldnames.h>
#include <Core.h>

dae::ScrollingBGComponent::ScrollingBGComponent( GameObject* pParent,
												 const std::string& path,
												 float speed,
												 ScrollingDir direction )
	: RenderComponent( pParent )
	, m_BackgroundTex( ResourceManager::GetInstance().LoadTexture( path ) )
	, m_Direction( direction )
{
	switch ( direction )
	{
	case ScrollingDir::up: {
		m_ScrollingVelocity = glm::vec2{ 0.f, -speed };
		break;
	}
	case ScrollingDir::down: {
		m_ScrollingVelocity = glm::vec2{ 0.f, speed };
		break;
	}
	case ScrollingDir::left: {
		m_ScrollingVelocity = glm::vec2{ -speed, 0.f };
		break;
	}
	case ScrollingDir::right: {
		m_ScrollingVelocity = glm::vec2{ speed, 0.f };
		break;
	}
	}
}

void dae::ScrollingBGComponent::Update()
{
	m_CurrentScroll += m_ScrollingVelocity * Timer::GetInstance().GetElapsed();

	// Wrap values
	if ( m_CurrentScroll.x > m_BackgroundTex->GetSize().x )
	{
		m_CurrentScroll.x -= m_BackgroundTex->GetSize().x;
	}
	if ( m_CurrentScroll.x < -m_BackgroundTex->GetSize().x )
	{
		m_CurrentScroll.x += m_BackgroundTex->GetSize().x;
	}
	if ( m_CurrentScroll.y > m_BackgroundTex->GetSize().y )
	{
		m_CurrentScroll.y -= m_BackgroundTex->GetSize().y;
	}
	if ( m_CurrentScroll.y < -m_BackgroundTex->GetSize().y )
	{
		m_CurrentScroll.y += m_BackgroundTex->GetSize().y;
	}
}

void dae::ScrollingBGComponent::Render() const
{
	Renderer::GetInstance().RenderTexture( *m_BackgroundTex, m_CurrentScroll.x, m_CurrentScroll.y );

	switch ( m_Direction )
	{
	case ScrollingDir::up: {
		Renderer::GetInstance().RenderTexture(
			*m_BackgroundTex, m_CurrentScroll.x, m_CurrentScroll.y + m_BackgroundTex->GetSize().y );
		break;
	}
	case ScrollingDir::down: {
		Renderer::GetInstance().RenderTexture(
			*m_BackgroundTex, m_CurrentScroll.x, m_CurrentScroll.y - m_BackgroundTex->GetSize().y );
		break;
	}
	case ScrollingDir::left: {
		Renderer::GetInstance().RenderTexture(
			*m_BackgroundTex, m_CurrentScroll.x + m_BackgroundTex->GetSize().x, m_CurrentScroll.y );
		break;
	}
	case ScrollingDir::right: {
		Renderer::GetInstance().RenderTexture(
			*m_BackgroundTex, m_CurrentScroll.x - m_BackgroundTex->GetSize().x, m_CurrentScroll.y );
		break;
	}
	}
}
