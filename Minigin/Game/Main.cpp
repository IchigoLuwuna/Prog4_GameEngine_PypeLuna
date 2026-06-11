#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game/Commands/MoveCommand.h"
#include "Input/InputManager.h"
#include "Random/Random.h"
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/PixelTextComponent.h"
#include "Components/TextAllignmentComponent.h"

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
	bgScene.Add( std::move( background ) );

	auto fps{ dae::functions::ui::MakeFpsCounter() };
	uiScene.Add( std::move( fps ) );
	//

	// Player Characters
	auto ship{ dae::functions::player::MakePlayer() };
	ship->GetComponent<dae::TransformComponent>()->MoveTo( 100.f, 192.f );
	gameScene.Add( std::move( ship ) );
	//

	// Enemies
	constexpr int zakoCount{ 18 };
	for ( int idx{}; idx < zakoCount; ++idx )
	{
		auto zako{ dae::functions::enemy::MakeZako() };
		zako->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( zako ) );
	}
	constexpr int goeiCount{ 14 };
	for ( int idx{}; idx < goeiCount; ++idx )
	{
		auto goei{ dae::functions::enemy::MakeGoei() };
		goei->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( goei ) );
	}
	constexpr int bossCount{ 5 };
	for ( int idx{}; idx < bossCount; ++idx )
	{
		auto boss{ dae::functions::enemy::MakeBoss() };
		boss->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( boss ) );
	}
	//

	// Scoreboard
	auto scoreboard{ dae::functions::ui::MakeScoreboard() };
	scoreboard->AddComponent<dae::TextAllignmentComponent>( glm::vec2{ 288.f, 0.f },
															dae::TextAllignmentComponent::Allignment::topRight );
	uiScene.Add( std::move( scoreboard ) );
	//

	// Add to scene
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
