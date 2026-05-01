#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Sound/SDLSoundService.h"
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/FpsComponent.h"
#include "Components/HealthComponent.h"
#include "Components/HealthDisplayComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ScoreDisplayComponent.h"
#include "Components/ReactiveSoundComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"

#include "Commands/EventCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/MoveCommand.h"

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
	background->AddComponent<dae::TextureComponent>( "./BG.png" );
	auto logo{ std::make_unique<dae::GameObject>() };
	logo->AddComponent<dae::TextureComponent>( "./logo.png" );
	auto text{ std::make_unique<dae::GameObject>() };
	auto bigFont{ dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 ) };
	text->AddComponent<dae::TextComponent>( "Programming 4 Assignment", bigFont, SDL_Color{ 255, 255, 0, 255 } );
	auto fps{ std::make_unique<dae::GameObject>() };
	fps->AddComponent<dae::TextComponent>( ".", bigFont );
	fps->AddComponent<dae::FpsComponent>();
	//

	// Player Characters
	auto operaBird{ std::make_unique<dae::GameObject>() };
	operaBird->AddComponent<dae::TextureComponent>( "./opera-bird.png" );
	operaBird->AddComponent<dae::HealthComponent>( 3u );
	operaBird->AddComponent<dae::ScoreComponent>();
	operaBird
		->AddComponent<dae::ReactiveSoundComponent>() // Easy to add sounds thanks to chaining
		.AddSound( { "e_SmallPelletPickup"_hash, operaBird.get(), "Data/wow.mp3" } )
		.AddSound( { "e_BigPelletPickup"_hash, operaBird.get(), "Data/wow.mp3" } )
		.AddSound( { "e_EntityDied"_hash, operaBird.get(), "Data/fuku2.mp3" } );

	auto dotoSheep{ std::make_unique<dae::GameObject>() };
	dotoSheep->AddComponent<dae::TextureComponent>( "./doto-sheep.png" );
	dotoSheep->AddComponent<dae::HealthComponent>( 3 );
	dotoSheep->AddComponent<dae::ScoreComponent>();
	dotoSheep->AddComponent<dae::ReactiveSoundComponent>()
		.AddSound( { "e_SmallPelletPickup"_hash, dotoSheep.get(), "Data/wow.mp3" } )
		.AddSound( { "e_BigPelletPickup"_hash, dotoSheep.get(), "Data/wow.mp3" } )
		.AddSound( { "e_EntityDied"_hash, dotoSheep.get(), "Data/fuku1.mp3" } );
	//

	// Scoreboard
	auto smallFont{ dae::ResourceManager::GetInstance().LoadFont( "Lingua.otf", 18 ) };

	auto operaInfoText{ std::make_unique<dae::GameObject>() };
	operaInfoText->AddComponent<dae::TextComponent>(
		"Use the D-Pad to move TM Opera O (bird), X to inflict damage, A and B to pick up pellets",
		smallFont,
		SDL_Color{ 255, 255, 255, 255 } );
	auto dotoInfoText{ std::make_unique<dae::GameObject>() };
	dotoInfoText->AddComponent<dae::TextComponent>(
		"Use WASD to move Meisho Doto (sheep), C to inflict damage, Z and X to pick up pellets ",
		smallFont,
		SDL_Color{ 255, 255, 255, 255 } );

	auto operaHealthDisplay{ std::make_unique<dae::GameObject>() };
	operaHealthDisplay->AddComponent<dae::TextComponent>( ".", smallFont, SDL_Color{ 255, 255, 255, 255 } );
	operaHealthDisplay->AddComponent<dae::HealthDisplayComponent>( operaBird->GetComponent<dae::HealthComponent>() );

	auto operaScoreDisplay{ std::make_unique<dae::GameObject>() };
	operaScoreDisplay->AddComponent<dae::TextComponent>( ".", smallFont, SDL_Color{ 255, 255, 255, 255 } );
	operaScoreDisplay->AddComponent<dae::ScoreDisplayComponent>( operaBird->GetComponent<dae::ScoreComponent>() );

	auto dotoHealthDisplay{ std::make_unique<dae::GameObject>() };
	dotoHealthDisplay->AddComponent<dae::TextComponent>( ".", smallFont, SDL_Color{ 255, 255, 255, 255 } );
	dotoHealthDisplay->AddComponent<dae::HealthDisplayComponent>( dotoSheep->GetComponent<dae::HealthComponent>() );

	auto dotoScoreDisplay{ std::make_unique<dae::GameObject>() };
	dotoScoreDisplay->AddComponent<dae::TextComponent>( ".", smallFont, SDL_Color{ 255, 255, 255, 255 } );
	dotoScoreDisplay->AddComponent<dae::ScoreDisplayComponent>( dotoSheep->GetComponent<dae::ScoreComponent>() );
	//

	// Create SceneGraph
	dotoInfoText->SetParent( operaInfoText.get() );
	operaHealthDisplay->SetParent( operaInfoText.get() );
	operaScoreDisplay->SetParent( operaInfoText.get() );
	dotoHealthDisplay->SetParent( operaInfoText.get() );
	dotoScoreDisplay->SetParent( operaInfoText.get() );
	//

	// Set Starting Positions
	logo->GetComponent<dae::TransformComponent>()->MoveTo( 358.f, 180.f );
	text->GetComponent<dae::TransformComponent>()->MoveTo( 292.f, 20.f );

	dotoSheep->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 150.f, 200.f } );
	operaBird->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 500.f, 200.f } );

	operaInfoText->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 10.f, 100.f } );
	dotoInfoText->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 24.f } );
	operaHealthDisplay->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 64.f } );
	operaScoreDisplay->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 88.f } );
	dotoHealthDisplay->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 112.f } );
	dotoScoreDisplay->GetComponent<dae::TransformComponent>()->MoveTo( glm::vec2{ 0.f, 136.f } );
	//

	// Create bindings
	constexpr float moveSpeed{ 150.f };
	// Opera Bird using controller
	const auto dUpKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::up ) };
	const auto dDownKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::down ) };
	const auto dLeftKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::left ) };
	const auto dRightKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::right ) };

	const auto southKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::south ) };
	const auto eastKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::east ) };
	const auto northKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::north ) };

	const auto pOperaTransform{ operaBird->GetComponent<dae::TransformComponent>() };
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dUpKey, dae::InputManager::KeyState::held, pOperaTransform, glm::vec2{ 0.f, -moveSpeed * 2.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dDownKey, dae::InputManager::KeyState::held, pOperaTransform, glm::vec2{ 0.f, moveSpeed * 2.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dLeftKey, dae::InputManager::KeyState::held, pOperaTransform, glm::vec2{ -moveSpeed * 2.f, 0.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dRightKey, dae::InputManager::KeyState::held, pOperaTransform, glm::vec2{ moveSpeed * 2.f, 0.f } );

	dae::InputManager::GetInstance().BindCommand<dae::DamageCommand>(
		northKey, dae::InputManager::KeyState::down, operaBird->GetComponent<dae::HealthComponent>(), 1 );

	dae::InputManager::GetInstance().BindCommand<dae::EventCommand>(
		southKey, dae::InputManager::KeyState::down, dae::Event{ "e_SmallPelletPickup"_hash, operaBird.get() } );
	dae::InputManager::GetInstance().BindCommand<dae::EventCommand>(
		eastKey, dae::InputManager::KeyState::down, dae::Event{ "e_BigPelletPickup"_hash, operaBird.get() } );
	//

	// Doto Sheep using keyboard
	auto pDotoTransform{ dotoSheep->GetComponent<dae::TransformComponent>() };
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_W, dae::InputManager::KeyState::held, pDotoTransform, glm::vec2{ 0.f, -moveSpeed } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_S, dae::InputManager::KeyState::held, pDotoTransform, glm::vec2{ 0.f, moveSpeed } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_A, dae::InputManager::KeyState::held, pDotoTransform, glm::vec2{ -moveSpeed, 0.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_D, dae::InputManager::KeyState::held, pDotoTransform, glm::vec2{ moveSpeed, 0.f } );

	dae::InputManager::GetInstance().BindCommand<dae::DamageCommand>(
		SDL_SCANCODE_C, dae::InputManager::KeyState::down, dotoSheep->GetComponent<dae::HealthComponent>(), 1 );

	dae::InputManager::GetInstance().BindCommand<dae::EventCommand>(
		SDL_SCANCODE_Z, dae::InputManager::KeyState::down, dae::Event{ "e_SmallPelletPickup"_hash, dotoSheep.get() } );
	dae::InputManager::GetInstance().BindCommand<dae::EventCommand>(
		SDL_SCANCODE_X, dae::InputManager::KeyState::down, dae::Event{ "e_BigPelletPickup"_hash, dotoSheep.get() } );
	//

#ifndef NDEBUG
	//  Attach names to objects when debugging
	background->AddComponent<dae::DebugComponent>( "background" );
	logo->AddComponent<dae::DebugComponent>( "logo" );
	text->AddComponent<dae::DebugComponent>( "text" );
	dotoSheep->AddComponent<dae::DebugComponent>( "dotoSheep" );
	operaBird->AddComponent<dae::DebugComponent>( "operaBird" );
	operaInfoText->AddComponent<dae::DebugComponent>( "operaInfoText" );
	dotoInfoText->AddComponent<dae::DebugComponent>( "dotoInfoText" );
	operaHealthDisplay->AddComponent<dae::DebugComponent>( "operaHealthDisplay" );
	dotoHealthDisplay->AddComponent<dae::DebugComponent>( "dotoHealthDisplay" );
	fps->AddComponent<dae::DebugComponent>( "fps" );
//
#endif

	// Add to scene
	scene.Add( std::move( background ) );
	scene.Add( std::move( logo ) );
	scene.Add( std::move( text ) );
	scene.Add( std::move( dotoSheep ) );
	scene.Add( std::move( operaBird ) );
	scene.Add( std::move( operaInfoText ) );
	scene.Add( std::move( dotoInfoText ) );
	scene.Add( std::move( operaHealthDisplay ) );
	scene.Add( std::move( operaScoreDisplay ) );
	scene.Add( std::move( dotoHealthDisplay ) );
	scene.Add( std::move( dotoScoreDisplay ) );
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
