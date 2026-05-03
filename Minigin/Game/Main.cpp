#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/FpsComponent.h"
#include "Components/PixelTextComponent.h"
#include "Game/Components/ScrollingBGComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/ProjectileAmmoComponent.h"

#include "Commands/DamageCommand.h"

#include "Achievement/Achievement.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	dae::ServiceLocator<dae::SoundService>::GetInstance().RegisterService( std::make_unique<dae::SDLSoundService>() );

#ifndef NDEBUG
	dae::ServiceLocator<dae::SoundService>::GetInstance().AddLayer<dae::DebugSoundService>();
#endif

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
	ship->GetComponent<dae::TransformComponent>()->MoveTo( 136.f, 200.f );
	ship->AddComponent<dae::SpriteSheetComponent>( "Ship.png", dae::SpriteSheet::SpriteSheetInfo{ 8, 3 } )
		.SetIndex( 6, 0 );
	ship->AddComponent<dae::HealthComponent>( 3 );
	ship->AddComponent<dae::ProjectileAmmoComponent>( 2 );
	//

	// Scoreboard
	//

	// Create SceneGraph
	//

	// Set Starting Positions
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
	//

#ifndef NDEBUG
	//  Attach names to objects when debugging
	background->AddComponent<dae::DebugComponent>( "background" );
	ship->AddComponent<dae::DebugComponent>( "ship" );
	fps->AddComponent<dae::DebugComponent>( "fps" );
//
#endif

	// Add to scene
	bgScene.Add( std::move( background ) );
	gameScene.Add( std::move( ship ) );
	uiScene.Add( std::move( fps ) );
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
