#include "GameStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Sound.h>
#include <Components.h>
#include <Random.h>
#include <Input.h>
#include <Commands.h>
#include "Game/Components/PixelTextComponent.h"
#include "Game/Components/SpriteAllignmentComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Game/Components/TextAllignmentComponent.h"
#include "Game/Functions/Enemy.h"
#include "Game/Functions/Player.h"
#include "Game/Functions/UI.h"
#include "Scene/SceneManager.h"
#include "Game/Context.h"
#include "States/StateMachine.h"

dae::GameStartState::GameStartState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameStartState::Update()
{
	if ( m_StartPressed )
	{
		return GetParent()->FindOrCreateState<GameEnterState>();
	}

	return nullptr;
}
void dae::GameStartState::Enter()
{
	m_StartPressed = false;

	const auto startKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::start ) };
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_RETURN, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		startKey, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );

	auto& uiScene{ SceneManager::GetInstance().GetScene( uiIdx ) };

	auto logo{ std::make_unique<GameObject>( "startScreenUI"_hash ) };
	logo->AddComponent<dae::SpriteSheetComponent>( "Logo.png", SpriteSheet::SpriteSheetInfo{ 1, 1 } );
	logo->AddComponent<dae::SpriteAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
														dae::SpriteAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( logo ) );
}
void dae::GameStartState::Exit()
{
	auto& uiScene{ SceneManager::GetInstance().GetScene( uiIdx ) };

	auto startScreenObjects{ uiScene.GetAllByTag( "startScreenUI"_hash ) };
	for ( auto& object : startScreenObjects )
	{
		object->MarkForRemoval();
	}
}

dae::GameEnterState::GameEnterState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameEnterState::Update()
{
	m_StateTime += dae::Timer::GetInstance().GetElapsed();

	if ( m_StateTime > m_StartSoundLength )
	{
		auto newState{ GetParent()->FindOrCreateState<GameStageTransitionState>() };
		reinterpret_cast<GameStageTransitionState*>( newState )->SetNextStageNr( 1 );
		return newState;
	}

	return nullptr;
}
void dae::GameEnterState::Enter()
{
	m_StateTime = 0.f;

	// Play Enter Sound
	ServiceLocator<SoundService>::GetInstance().GetService().Play( m_StartSoundFile.c_str(), 1.f );

	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	auto scoreboard{ dae::functions::ui::MakeScoreboard() };
	scoreboard->AddComponent<dae::TextAllignmentComponent>( glm::vec2{ 288.f, 0.f },
															dae::TextAllignmentComponent::Allignment::topRight );
	uiScene.Add( std::move( scoreboard ) );

	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };

	// Player Characters
	auto ship{ dae::functions::player::MakePlayer() };
	ship->GetComponent<dae::TransformComponent>()->MoveTo( 288.f / 2.f, 192.f );
	gameScene.Add( std::move( ship ) );
	//

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto enterText{ std::make_unique<GameObject>( "stageEnterText"_hash ) };
	enterText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "Game Start!" );
	enterText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( enterText ) );
}
void dae::GameEnterState::Exit()
{
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	uiScene.GetByTag( "stageEnterText"_hash )->MarkForRemoval();
}

const std::string dae::GameStageTransitionState::m_TransitionSoundFile = "challenging_stage_start.wav";
dae::GameStageTransitionState::GameStageTransitionState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameStageTransitionState::Update()
{
	m_StateTime += Timer::GetInstance().GetElapsed();

	if ( m_StateTime > m_TransitionLength )
	{
		auto newState{ GetParent()->FindOrCreateState<GameStageState>() };
		reinterpret_cast<GameStageState*>( newState )->SetStageNr( m_NextStageNr );
		return newState;
	}

	return nullptr;
}
void dae::GameStageTransitionState::Enter()
{
	m_StateTime = 0.f;

	if ( m_NextStageNr != 1 )
	{
		ServiceLocator<SoundService>::GetInstance().GetService().Play( m_TransitionSoundFile.c_str(), 1.f );
	}

	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto enterText{ std::make_unique<GameObject>( "stageTransText"_hash ) };
	enterText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( std::format( "Stage {}", m_NextStageNr ) );
	enterText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( enterText ) );
}
void dae::GameStageTransitionState::Exit()
{
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };
	uiScene.GetByTag( "stageTransText"_hash )->MarkForRemoval();
}
void dae::GameStageTransitionState::SetNextStageNr( size_t nr )
{
	m_NextStageNr = nr;
}

dae::GameStageState::GameStageState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameStageState::Update()
{
	constexpr float checkInterval{ 1.f };
	m_TimeSinceLastCheck += Timer::GetInstance().GetElapsed();

	if ( m_StageNr == -1u )
	{
		return GetParent()->FindOrCreateState<GameStartState>();
	}

	if ( m_TimeSinceLastCheck < checkInterval )
	{
		return nullptr;
	}

	m_TimeSinceLastCheck = 0.f;

	if ( !m_HiveMind.IsFormationClear() )
	{
		return nullptr;
	}

	if ( m_StageNr == m_StageCount )
	{
		return GetParent()->FindOrCreateState<GameAllStagesClearState>();
	}

	auto newState{ GetParent()->FindOrCreateState<GameStageTransitionState>() };
	reinterpret_cast<GameStageTransitionState*>( newState )->SetNextStageNr( m_StageNr + 1 );
	return newState;
}
void dae::GameStageState::Enter()
{
	m_TimeSinceLastCheck = 0.f;

	if ( m_StageNr == -1u || m_StageNr > m_StageCount )
	{
		m_StageNr = -1u;
		return;
	}

	size_t stageIdx{ m_StageNr - 1 };

	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };

	// Enemies
	const uint32_t zakoCount{ m_StageEnemyCounts[stageIdx][0] };
	for ( uint32_t idx{}; idx < zakoCount; ++idx )
	{
		auto zako{ dae::functions::enemy::MakeZako() };
		zako->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( zako ) );
	}
	const uint32_t goeiCount{ m_StageEnemyCounts[stageIdx][1] };
	for ( uint32_t idx{}; idx < goeiCount; ++idx )
	{
		auto goei{ dae::functions::enemy::MakeGoei() };
		goei->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( goei ) );
	}
	const uint32_t bossCount{ m_StageEnemyCounts[stageIdx][2] };
	for ( uint32_t idx{}; idx < bossCount; ++idx )
	{
		auto boss{ dae::functions::enemy::MakeBoss() };
		boss->GetComponent<dae::TransformComponent>()->MoveTo( dae::random::GetRand( 0.f, 288.f - 16.f ), -64.f );
		gameScene.Add( std::move( boss ) );
	}
	//
}
void dae::GameStageState::Exit()
{
}
void dae::GameStageState::SetStageNr( size_t nr )
{
	m_StageNr = nr;
}

dae::GameAllStagesClearState::GameAllStagesClearState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameAllStagesClearState::Update()
{
	m_StateTime += Timer::GetInstance().GetElapsed();

	if ( m_StateTime > m_TransitionLength )
	{
		return GetParent()->FindOrCreateState<GameStartState>();
	}

	return nullptr;
}
void dae::GameAllStagesClearState::Enter()
{
	m_StateTime = 0.f;

	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto enterText{ std::make_unique<GameObject>( "stageAllClearText"_hash ) };
	enterText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "All Stages Clear!" );
	enterText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( enterText ) );
}
void dae::GameAllStagesClearState::Exit()
{
	auto& gameScene{ dae::SceneManager::GetInstance().GetScene( gameIdx ) };
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	gameScene.GetByTag( "player"_hash )->MarkForRemoval();
	uiScene.GetByTag( "stageAllClearText"_hash )->MarkForRemoval();
	uiScene.GetByTag( "scoreboard"_hash )->MarkForRemoval();
}
