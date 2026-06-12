#include "Renderer.h"
#include <stdexcept>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "Scene/SceneManager.h"
#include "Helpers/Texture2D.h"

void dae::Renderer::Init( SDL_Window* window )
{
	m_Window = window;

	SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" );

#if defined( __EMSCRIPTEN__ )
	m_Renderer = SDL_CreateRenderer( window, nullptr );
#else
	m_Renderer = SDL_CreateRenderer( window, nullptr );
#endif

	if ( m_Renderer == nullptr )
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error( std::string( "SDL_CreateRenderer Error: " ) + SDL_GetError() );
	}

	// Do ImGui stuff
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable gamepad
#ifdef __EMSCRIPTEN__
	io.IniFilename = NULL;
#endif
	ImGui_ImplSDL3_InitForSDLRenderer( window, m_Renderer );
	ImGui_ImplSDLRenderer3_Init( m_Renderer );
}

void dae::Renderer::Render() const
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor( m_Renderer, color.r, color.g, color.b, color.a );
	SDL_RenderClear( m_Renderer );

	SceneManager::GetInstance().Render();

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

	SDL_RenderPresent( m_Renderer );
}

void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if ( m_Renderer != nullptr )
	{
		SDL_DestroyRenderer( m_Renderer );
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const float x, const float y ) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize( texture.GetSDLTexture(), &dst.w, &dst.h );
	SDL_RenderTexture( GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst );
}

void dae::Renderer::RenderTexture(
	const Texture2D& texture, const float x, const float y, const float width, const float height ) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture( GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst );
}

void dae::Renderer::RenderTexture( const Texture2D& texture, const SDL_FRect& srcRect, const SDL_FRect& dstRect ) const
{
	SDL_RenderTexture( GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect );
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const
{
	return m_Renderer;
}
