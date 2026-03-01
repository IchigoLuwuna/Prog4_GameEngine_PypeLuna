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

	// Initialize objects
	auto background{ std::make_unique<dae::GameObject>() };
	background->AddComponent<dae::TextureComponent>( "./background.png" );

	auto logo{ std::make_unique<dae::GameObject>() };
	logo->GetComponent<dae::TransformComponent>()->MoveTo( 358.f, 180.f );
	logo->AddComponent<dae::TextureComponent>( "./logo.png" );

	auto text{ std::make_unique<dae::GameObject>() };
	text->GetComponent<dae::TransformComponent>()->MoveTo( 292.f, 20.f );
	auto font{ dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	text->AddComponent<dae::TextComponent>( "Programming 4 Assignment", font, SDL_Color{ 255, 255, 0, 255 } );

	auto fps{ std::make_unique<dae::GameObject>() };
	fps->AddComponent<dae::TextComponent>( ".", font );
	fps->AddComponent<dae::FpsComponent>();

	auto orbitCenter{ std::make_unique<dae::GameObject>() };
	orbitCenter->GetComponent<dae::TransformComponent>()->MoveTo( 350.f, 250.f );

	auto dotoSheep{ std::make_unique<dae::GameObject>() };
	dotoSheep->AddComponent<dae::TextureComponent>( "./doto-sheep.png" );
	dotoSheep->AddComponent<dae::OrbitMovementComponent>( 50.f, -std::numbers::pi );

	auto operaBird{ std::make_unique<dae::GameObject>() };
	operaBird->AddComponent<dae::TextureComponent>( "./opera-bird.png" );
	operaBird->AddComponent<dae::OrbitMovementComponent>( 25.f, 1.5f * std::numbers::pi );

	// Create SceneGraph
	dotoSheep->SetParent( orbitCenter.get() );
	operaBird->SetParent( dotoSheep.get() );

	// Add to scene
	scene.Add( std::move( background ) );
	scene.Add( std::move( logo ) );
	scene.Add( std::move( text ) );
	scene.Add( std::move( fps ) );
	scene.Add( std::move( orbitCenter ) );
	scene.Add( std::move( dotoSheep ) );
	scene.Add( std::move( operaBird ) );
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
