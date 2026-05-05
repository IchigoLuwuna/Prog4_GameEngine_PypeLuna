#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Game/Components/AnimationComponent.h"
#include "Components/FpsComponent.h"
#include "Components/PixelTextComponent.h"
#include "Components/ScrollingBGComponent.h"
#include "Components/SpriteSheetComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ProjectileAmmoComponent.h"
#include "Components/TextAllignmentComponent.h"
#include "Components/ScoreDisplayComponent.h"
#include "Components/ReactiveSoundComponent.h"
#include "Components/StateComponent.h"

#include "Commands/DamageCommand.h"

#include "Game/States/ZakoStates.h"

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

	[[maybe_unused]] constexpr size_t bgIdx{ 0 };
	[[maybe_unused]] constexpr size_t gameIdx{ 1 };
	[[maybe_unused]] constexpr size_t uiIdx{ 2 };
	auto& bgScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& gameScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& uiScene{ dae::SceneManager::GetInstance().CreateScene() };

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
	auto ship{ std::make_unique<dae::GameObject>() };
	ship->AddComponent<dae::SpriteSheetComponent>( "Ship.png", dae::SpriteSheet::SpriteSheetInfo{ 8, 3 } )
		.SetIndex( 6, 0 );
	ship->AddComponent<dae::HealthComponent>( 3 );
	std::vector<std::pair<size_t, uint32_t>> shipScoreGainOnEvent{ { "e_InsectDied"_hash, 100 } };
	ship->AddComponent<dae::ScoreComponent>( std::move( shipScoreGainOnEvent ) );
	ship->AddComponent<dae::ProjectileAmmoComponent>( 2 );
	ship->AddComponent<dae::ReactiveSoundComponent>().AddSound( { "e_EntityDied"_hash, ship.get(), "explosion.wav" } );
	//

	// Enemies
	auto zako{ std::make_unique<dae::GameObject>() };
	zako->AddComponent<dae::SpriteSheetComponent>( "Enemy.png", dae::SpriteSheet::SpriteSheetInfo{ 24, 3 } );
	zako->AddComponent<dae::AnimationComponent>()
		.AddAnimation( "anim_Idle"_hash, { 6, 7, 0.25f, true } )
		.SetAnimation( "anim_Idle"_hash );
	zako->AddComponent<dae::HealthComponent>( 1 );
	zako->AddComponent<dae::StateComponent<dae::ZakoState>>().SetState<dae::ZakoIdlingState>();
	zako->AddComponent<dae::ReactiveSoundComponent>().AddSound( { "e_EntityDied"_hash, zako.get(), "zako_destroy" } );
	//

	// Scoreboard
	auto playerScoreBoard{ std::make_unique<dae::GameObject>() };
	playerScoreBoard->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } );
	playerScoreBoard->AddComponent<dae::ScoreDisplayComponent>().SetSubjectScore(
		ship->GetComponent<dae::ScoreComponent>() );
	//

	// Create SceneGraph
	//

	// Set Starting Positions
	ship->GetComponent<dae::TransformComponent>()->MoveTo( 136.f, 200.f );
	zako->GetComponent<dae::TransformComponent>()->MoveTo( 136.f, 8.f );
	playerScoreBoard->AddComponent<dae::TextAllignmentComponent>( glm::vec2{ 288.f, 0.f },
																  dae::TextAllignmentComponent::Allignment::topRight );
	//

	// Create bindings
	// Functions
	auto shipPosRef{ ship->GetComponent<dae::TransformComponent>() };
	auto shipAmmoRef{ ship->GetComponent<dae::ProjectileAmmoComponent>() };
	auto moveLeft{ [=]() mutable {
		constexpr float movement{ -96.f };

		if ( !shipPosRef.Validate() )
		{
			return;
		}
		if ( shipPosRef->GetPosition().x + movement * dae::Timer::GetInstance().GetElapsed() <= 0.f )
		{
			shipPosRef->MoveTo( 0.f, shipPosRef->GetPosition().y );
			return;
		}
		shipPosRef->Move( movement * dae::Timer::GetInstance().GetElapsed(), 0.f );
	} };
	auto moveRight{ [=]() mutable {
		constexpr float movement{ 96.f };

		if ( !shipPosRef.Validate() )
		{
			return;
		}
		if ( shipPosRef->GetPosition().x + 15.f + movement * dae::Timer::GetInstance().GetElapsed() >= 288.f )
		{
			shipPosRef->MoveTo( 288.f - 15.f, shipPosRef->GetPosition().y );
			return;
		}
		shipPosRef->Move( movement * dae::Timer::GetInstance().GetElapsed(), 0.f );
	} };
	auto shoot{ [=]() mutable {
		if ( !shipPosRef.Validate() )
		{
			return;
		}
		if ( !shipAmmoRef.Validate() )
		{
			return;
		}
		if ( !shipAmmoRef->HasAmmo() )
		{
			return;
		}

		shipAmmoRef->DecreaseAmmo();
		dae::ServiceLocator<dae::SoundService>::GetInstance().GetService().Play( "fire.wav", 1.f );

		auto projectile{ std::make_unique<dae::GameObject>() };
		projectile
			->AddComponent<dae::SpriteSheetComponent>( "Projectile.png", dae::SpriteSheet::SpriteSheetInfo{ 7, 1 } )
			.SetIndex( 3 );
		projectile->AddComponent<dae::ProjectileComponent>( glm::vec2{ 0.f, -256.f }, 0.8f )
			.RegisterObserver( shipAmmoRef );
		projectile->GetComponent<dae::TransformComponent>()->MoveTo( shipPosRef->GetPosition() + glm::vec2{ 4, 0 } );

		dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( projectile ) );
	} };

	// Bindings
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_A, dae::InputManager::KeyState::held, moveLeft );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_LEFT, dae::InputManager::KeyState::held, moveLeft );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_D, dae::InputManager::KeyState::held, moveRight );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_RIGHT, dae::InputManager::KeyState::held, moveRight );

	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_SPACE, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_J, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_K, dae::InputManager::KeyState::down, shoot );

	dae::InputManager::GetInstance().BindCommand<dae::EventCommand>(
		SDL_SCANCODE_Z, dae::InputManager::KeyState::down, dae::Event{ "e_InsectDied"_hash, nullptr } );

	dae::InputManager::GetInstance().BindCommand<dae::DamageCommand>(
		SDL_SCANCODE_U, dae::InputManager::KeyState::down, ship->GetComponent<dae::HealthComponent>(), 1 );
	//

#ifndef NDEBUG
	//  Attach names to objects when debugging
	background->AddComponent<dae::DebugComponent>( "background" );
	ship->AddComponent<dae::DebugComponent>( "ship" );
	zako->AddComponent<dae::DebugComponent>( "zako" );
	fps->AddComponent<dae::DebugComponent>( "fps" );
	playerScoreBoard->AddComponent<dae::DebugComponent>( "playerScoreBoard" );
//
#endif

	// Add to scene
	bgScene.Add( std::move( background ) );
	gameScene.Add( std::move( ship ) );
	gameScene.Add( std::move( zako ) );
	uiScene.Add( std::move( fps ) );
	uiScene.Add( std::move( playerScoreBoard ) );
	//

#ifndef NDEBUG
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
