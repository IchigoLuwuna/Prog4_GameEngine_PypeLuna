#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene{ dae::SceneManager::GetInstance().CreateScene() };

	auto gameObject{ std::make_unique<dae::GameObject>() };
	gameObject->SetTexture( "background.png" );
	scene.Add( std::move( gameObject ) );

	gameObject = std::make_unique<dae::GameObject>();
	gameObject->SetTexture( "logo.png" );
	gameObject->SetPosition( 358, 180 );
	scene.Add( std::move( gameObject ) );

	auto font = dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	auto textObject = std::make_unique<dae::TextObject>( "Programming 4 Assignment", font );
	textObject->SetColor( { 255, 255, 0, 255 } );
	textObject->SetPosition( 292, 20 );
	scene.Add( std::move( textObject ) );
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
