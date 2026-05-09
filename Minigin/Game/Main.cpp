#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/PixelTextComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/TextAllignmentComponent.h"
#include "Components/ScoreDisplayComponent.h"

#include "Functions/Enemy.h"
#include "Game/Functions/Player.h"
#include "Game/Functions/UI.h"

#include "States/ZakoStates.h"

#include "Achievement/Achievement.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	dae::ServiceLocator<dae::SoundService>::GetInstance().RegisterService( std::make_unique<dae::SDLSoundService>() );

#ifndef NDEBUG
	dae::ServiceLocator<dae::SoundService>::GetInstance().AddLayer<dae::DebugSoundService>();
#endif

	dae::ServiceLocator<dae::SoundService>::GetInstance().GetService().Play( "start.wav", 1.f );

	auto& bgScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& gameScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& uiScene{ dae::SceneManager::GetInstance().CreateScene() };

	// Initialize objects
	// Base
	auto background{ dae::functions::ui::MakeBackground() };
	auto fps{ dae::functions::ui::MakeFpsCounter() };
	//

	// Player Characters
	auto ship{ dae::functions::player::MakePlayer() };
	//

	// Enemies
	auto zako1{ dae::functions::enemy::MakeZako() };
	auto zako2{ dae::functions::enemy::MakeZako() };
	auto zako3{ dae::functions::enemy::MakeZako() };
	//

	// Scoreboard
	auto scoreboard{ dae::functions::ui::MakeScoreboard( ship.get() ) };
	//

	// Set Starting Positions
	ship->GetComponent<dae::TransformComponent>()->MoveTo( 100.f, 200.f );
	zako1->GetComponent<dae::TransformComponent>()->MoveTo( 68.f, -64.f );
	zako2->GetComponent<dae::TransformComponent>()->MoveTo( 136.f, -64.f );
	zako3->GetComponent<dae::TransformComponent>()->MoveTo( 204.f, -64.f );
	scoreboard->AddComponent<dae::TextAllignmentComponent>( glm::vec2{ 288.f, 0.f },
															dae::TextAllignmentComponent::Allignment::topRight );
	//

#ifndef NDEBUG
	//  Attach names to objects when debugging
	background->AddComponent<dae::DebugComponent>( "background" );
	ship->AddComponent<dae::DebugComponent>( "ship" );
	zako1->AddComponent<dae::DebugComponent>( "zako" );
	zako2->AddComponent<dae::DebugComponent>( "zako" );
	zako3->AddComponent<dae::DebugComponent>( "zako" );
	fps->AddComponent<dae::DebugComponent>( "fps" );
	scoreboard->AddComponent<dae::DebugComponent>( "scoreboard" );
//
#endif

	// Add to scene
	bgScene.Add( std::move( background ) );
	gameScene.Add( std::move( ship ) );
	gameScene.Add( std::move( zako1 ) );
	gameScene.Add( std::move( zako2 ) );
	gameScene.Add( std::move( zako3 ) );
	uiScene.Add( std::move( fps ) );
	uiScene.Add( std::move( scoreboard ) );

	bgScene.AddRequested();
	gameScene.AddRequested();
	uiScene.AddRequested();
	//

#ifndef NDEBUG
	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	// Control for debug mode
	auto controlHints{ std::make_unique<dae::GameObject>() };
	controlHints->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "WASD or arrows for movement\nSPACE J and K for shooting\nZ to give yourself points\nENTER to hide "
				  "this message" );

	controlHints->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 8.f } );
	controlHints->AddComponent<dae::DebugComponent>( "controlHints" );

	dae::Validator controlHintsValidator{ controlHints->GetComponent<dae::TransformComponent>().GetControlBlock() };
	auto* pControlHints{ controlHints.get() };
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_RETURN, dae::InputManager::KeyState::down, [=]() mutable {
			if ( !controlHintsValidator.Validate() )
			{
				return;
			}
			pControlHints->MarkForRemoval();
		} );

	uiScene.Add( std::move( controlHints ) );
#endif

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
