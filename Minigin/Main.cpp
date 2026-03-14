#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Commands.h"
#include "Gamepad.h"
#include "ImGuiComponents.h"
#include "InputManager.h"

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

	auto dotoSheep{ std::make_unique<dae::GameObject>() };
	dotoSheep->AddComponent<dae::TextureComponent>( "./doto-sheep.png" );

	auto operaBird{ std::make_unique<dae::GameObject>() };
	operaBird->AddComponent<dae::TextureComponent>( "./opera-bird.png" );

	// Create SceneGraph

	// Register Observers To Subjects

	// Create bindings
	// Doto Sheep using keyboard
	constexpr float moveSpeed{ 150.f };
	auto pDotoSheepTransform{ dotoSheep->GetComponent<dae::TransformComponent>() };
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_W, dae::InputManager::KeyState::held, pDotoSheepTransform, glm::vec2{ 0.f, -moveSpeed } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_S, dae::InputManager::KeyState::held, pDotoSheepTransform, glm::vec2{ 0.f, moveSpeed } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_A, dae::InputManager::KeyState::held, pDotoSheepTransform, glm::vec2{ -moveSpeed, 0.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		SDL_SCANCODE_D, dae::InputManager::KeyState::held, pDotoSheepTransform, glm::vec2{ moveSpeed, 0.f } );
	//

	// Opera Bird using controller
	const auto dUpKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::up ) };
	const auto dDownKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::down ) };
	const auto dLeftKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::left ) };
	const auto dRightKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::right ) };
	auto pOperaBirdTransform{ operaBird->GetComponent<dae::TransformComponent>() };
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dUpKey, dae::InputManager::KeyState::held, pOperaBirdTransform, glm::vec2{ 0.f, -moveSpeed * 2.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dDownKey, dae::InputManager::KeyState::held, pOperaBirdTransform, glm::vec2{ 0.f, moveSpeed * 2.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dLeftKey, dae::InputManager::KeyState::held, pOperaBirdTransform, glm::vec2{ -moveSpeed * 2.f, 0.f } );
	dae::InputManager::GetInstance().BindCommand<dae::MoveCommand>(
		dRightKey, dae::InputManager::KeyState::held, pOperaBirdTransform, glm::vec2{ moveSpeed * 2.f, 0.f } );
	//

	// Test/Showcase down/up bindings
	dae::InputManager::GetInstance().BindCommand<dae::LogCommand>(
		SDL_SCANCODE_RETURN, dae::InputManager::KeyState::down, "Enter Pressed" );
	dae::InputManager::GetInstance().BindCommand<dae::LogCommand>(
		SDL_SCANCODE_BACKSPACE, dae::InputManager::KeyState::up, "Backspace Released" );

	const auto startKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::start ) };
	const auto selectKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::select ) };
	dae::InputManager::GetInstance().BindCommand<dae::LogCommand>(
		startKey, dae::InputManager::KeyState::down, "Start Pressed" );
	dae::InputManager::GetInstance().BindCommand<dae::LogCommand>(
		selectKey, dae::InputManager::KeyState::up, "Select Released" );

	// Add to scene
	scene.Add( std::move( background ) );
	scene.Add( std::move( logo ) );
	scene.Add( std::move( text ) );
	scene.Add( std::move( dotoSheep ) );
	scene.Add( std::move( operaBird ) );
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
