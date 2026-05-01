#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/FpsComponent.h"
#include "Components/PixelTextComponent.h"
#include "Game/Components/ScrollingBGComponent.h"

#include "Achievement/Achievement.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	dae::ServiceLocator<dae::SoundService>::GetInstance().RegisterService( std::make_unique<dae::SDLSoundService>() );

#ifndef NDEBUG
	dae::ServiceLocator<dae::SoundService>::GetInstance().AddLayer<dae::DebugSoundService>();
#endif

	auto& scene{ dae::SceneManager::GetInstance().CreateScene() };

	// Initialize objects
	// Base
	auto background{ std::make_unique<dae::GameObject>() };
	background->AddComponent<dae::ScrollingBGComponent>(
		"BG.png", 64.f, dae::ScrollingBGComponent::ScrollingDir::down );
	auto bigFont{ dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 48 ) };
	auto fps{ std::make_unique<dae::GameObject>() };
	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };
	fps->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true );
	fps->AddComponent<dae::FpsComponent>();
	//

	// Player Characters
	//

	// Scoreboard
	//

	// Create SceneGraph
	//

	// Set Starting Positions
	//

	// Create bindings
	//

#ifndef NDEBUG
	//  Attach names to objects when debugging
	background->AddComponent<dae::DebugComponent>( "background" );
	fps->AddComponent<dae::DebugComponent>( "fps" );
//
#endif

	// Add to scene
	scene.Add( std::move( background ) );
	scene.Add( std::move( fps ) );
	//

	// Attach achievement handler
	dae::Minigin::eventManager.AttachListener( nullptr, dae::achievements::HandleEvent );
	//
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

#ifndef NDEBUG
	std::cout << "Game is running in debug mode, extra info will be displayed in the console\n";
#endif

	// https://www.arcade-museum.com/tech-center/machine/galaga
	dae::Minigin engine( { "Galaga", { 1388, 1080 }, { 288, 224 } }, data_location );
	engine.Run( load );

	return 0;
}
