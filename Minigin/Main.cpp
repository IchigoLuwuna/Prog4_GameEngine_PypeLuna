#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Components.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene() };

	/*
	auto fpsDisplay{ std::make_unique<dae::FpsDisplay>() };
	scene.Add( std::move( fpsDisplay ) );
	*/

	auto background{ std::make_unique<dae::GameObject>() };
	background->AddComponent<dae::TransformComponent>();
	background->AddComponent<dae::TextureComponent>( "./background.png" );
	scene.Add( std::move( background ) );

	auto logo{ std::make_unique<dae::GameObject>() };
	logo->AddComponent<dae::TransformComponent>( 358, 180 );
	logo->AddComponent<dae::TextureComponent>( "./logo.png" );
	scene.Add( std::move( logo ) );

	auto text{ std::make_unique<dae::GameObject>() };
	text->AddComponent<dae::TransformComponent>( 292, 20 );
	auto font{ dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	text->AddComponent<dae::TextComponent>( "Programming 4 Assignment", font, SDL_Color{ 255, 255, 0, 255 } );
	scene.Add( std::move( text ) );

	auto fps{ std::make_unique<dae::GameObject>() };
	fps->AddComponent<dae::TransformComponent>();
	fps->AddComponent<dae::TextComponent>( ".", font );
	fps->AddComponent<dae::FpsComponent>();
	scene.Add( std::move( fps ) );
}

int main( int, char*[] )
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if ( !fs::exists( data_location ) )
		data_location = "../Data/";
#endif
	dae::Minigin engine( data_location );
	engine.Run( load );
	return 0;
}
