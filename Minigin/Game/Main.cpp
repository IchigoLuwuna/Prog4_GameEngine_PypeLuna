#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game/Commands/MoveCommand.h"
#include "Game/Components/GameStateHandlerComponent.h"
#include "Game/Components/TextAllignmentComponent.h"
#include "Game/Functions/Score.h"
#include "Game/States/GameStates.h"
#include <Input.h>
#include <Random.h>
#if _DEBUG && __has_include( <vld.h>)
#	include <vld.h>
#endif

#include <Engine.h>

#include "Components/PixelTextComponent.h"

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

	auto& levelScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& bgScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& gameScene{ dae::SceneManager::GetInstance().CreateScene() };
	auto& uiScene{ dae::SceneManager::GetInstance().CreateScene() };

	auto levelMan{ std::make_unique<dae::GameObject>( "levelMan"_hash ) };
	levelMan->RemoveComponent<dae::TransformComponent>();
	levelMan->AddComponent<dae::GameStateHandlerComponent>().SetState<dae::GameStartState>();
	levelScene.Add( std::move( levelMan ) );

	// Initialize objects
	auto background{ dae::functions::ui::MakeBackground() };
	bgScene.Add( std::move( background ) );

	auto fps{ dae::functions::ui::MakeFpsCounter() };
	uiScene.Add( std::move( fps ) );
	//

	// Add to scene
	bgScene.AddRequested();
	gameScene.AddRequested();
	uiScene.AddRequested();
	//

	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_F2, dae::InputManager::KeyState::down, []() {
			static bool mute{};
			mute = !mute;
			if ( mute )
			{
				dae::ServiceLocator<dae::SoundService>::GetInstance().GetService().Mute();
			}
			else
			{
				dae::ServiceLocator<dae::SoundService>::GetInstance().GetService().UnMute();
			}
		} );

#ifndef NDEBUG
	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	// Control for debug mode
	auto controlHints{ std::make_unique<dae::GameObject>() };
	controlHints->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "A and D for movement\nJ or K for shooting\nBACK to hide this message" );

	controlHints->AddComponent<dae::TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f + 64.f },
															  dae::TextAllignmentComponent::Allignment::center );
	controlHints->AddComponent<dae::DebugComponent>( "controlHints" );

	dae::Validator controlHintsValidator{ controlHints->GetComponent<dae::TransformComponent>().GetControlBlock() };
	auto* pControlHints{ controlHints.get() };
	auto removeHelpText{ [=]() mutable {
		if ( !controlHintsValidator.Validate() )
		{
			return;
		}
		pControlHints->MarkForRemoval();
	} };
	const auto selectKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::select ) };
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_BACKSPACE, dae::InputManager::KeyState::down, removeHelpText );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		selectKey, dae::InputManager::KeyState::down, removeHelpText );

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
	dae::Minigin engine( { "Galaga", { 926, 720 }, { 288, 224 } }, data_location );
	engine.Run( load );

	return 0;
}
