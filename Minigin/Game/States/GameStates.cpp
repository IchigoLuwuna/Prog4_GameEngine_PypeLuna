#include "GameStates.h"
#include <Helpers.h>
#include <Patterns.h>
#include <Sound.h>
#include <Components.h>
#include <Random.h>
#include <Input.h>
#include <Commands.h>
#include "Game/Components/PixelTextComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/SpriteAllignmentComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Game/Components/TextAllignmentComponent.h"
#include "Game/Functions/Enemy.h"
#include "Game/Functions/Player.h"
#include "Game/Functions/Score.h"
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

	auto triggersState{ dae::InputManager::GetInstance().PollAxis( Gamepad::Axis::triggers ) };
	bool activateEasterEgg{ ( dae::InputManager::GetInstance().PollKey( SDL_SCANCODE_T ) &&
							  dae::InputManager::GetInstance().PollKey( SDL_SCANCODE_Y ) ) ||
							( triggersState.first > 20'000 && triggersState.second > 20'000 ) };

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
	if ( activateEasterEgg )
	{
		ship->GetComponent<dae::SpriteSheetComponent>()->SetIndex( 7, 1 );
	}
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
	, m_Subscription( this, std::bind( &GameStageState::HandleEvent, this, std::placeholders::_1 ) )
{
}
dae::State* dae::GameStageState::Update()
{
	constexpr float checkInterval{ 1.f };
	m_TimeSinceLastCheck += Timer::GetInstance().GetElapsed();

	if ( m_PlayerRanOutOfLives )
	{
		if ( m_GameOverTime <= 0.f )
		{
			return GetParent()->FindOrCreateState<GameGameOverState>();
		}
		m_GameOverTime -= Timer::GetInstance().GetElapsed();
		return nullptr;
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
	m_GameOverTime = 3.f;
	m_PlayerRanOutOfLives = false;

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
void dae::GameStageState::HandleEvent( Event& event )
{
	switch ( event.eventHash )
	{
	case "e_ShipRanOutOfLives"_hash: {
		m_PlayerRanOutOfLives = true;
	}
	default: {
		break;
	}
	}
}

dae::GameGameOverState::GameGameOverState( StateMachine* pParent )
	: State( pParent )
{
}
dae::GameGameOverState::State* dae::GameGameOverState::Update()
{
	m_StateTime += Timer::GetInstance().GetElapsed();

	if ( m_StateTime > m_TransitionLength )
	{
		return GetParent()->FindOrCreateState<GameScoreboardState>();
	}

	constexpr float fadeoutTime{ 0.05f };
	if ( !m_SoundPlayed && m_StateTime > fadeoutTime )
	{
		ServiceLocator<SoundService>::GetInstance().GetService().Play( "fighter_captured.wav", 1.f );
		m_SoundPlayed = true;
	}

	return nullptr;
}
void dae::GameGameOverState::Enter()
{
	m_StateTime = 0.f;
	m_SoundPlayed = false;

	// Flush game
	auto& gameScene{ dae::SceneManager::GetInstance().GetScene( gameIdx ) };
	gameScene.MarkAllAsRemovable();

	ServiceLocator<SoundService>::GetInstance().GetService().StopAll();

	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto enterText{ std::make_unique<GameObject>( "gameOverText"_hash ) };
	enterText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "Game Over" );
	enterText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( enterText ) );
}
void dae::GameGameOverState::Exit()
{
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	uiScene.GetByTag( "gameOverText"_hash )->MarkForRemoval();
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
		return GetParent()->FindOrCreateState<GameScoreboardState>();
	}

	return nullptr;
}
void dae::GameAllStagesClearState::Enter()
{
	m_StateTime = 0.f;

	ServiceLocator<SoundService>::GetInstance().GetService().StopAll();
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "challenging_stage_perfect.wav", 1.f );

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
	auto& levelScene{ SceneManager::GetInstance().GetScene( levelIdx ) };
	auto& gameScene{ dae::SceneManager::GetInstance().GetScene( gameIdx ) };
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	auto pPlayer{ gameScene.GetByTag( "player"_hash ) };
	auto scoreTransferObject{ std::make_unique<GameObject>( "scoreTransferObject"_hash ) };
	scoreTransferObject->AddComponent<ScoreComponent>( std::vector<std::pair<size_t, uint32_t>>{},
													   pPlayer->GetComponent<ScoreComponent>()->GetScore() );
	pPlayer->MarkForRemoval();
	levelScene.Add( std::move( scoreTransferObject ) );

	uiScene.GetByTag( "stageAllClearText"_hash )->MarkForRemoval();

	// Flush game
	gameScene.MarkAllAsRemovable();
}

dae::GameScoreboardState::GameScoreboardState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameScoreboardState::Update()
{
	if ( !m_StartPressed )
	{
		return nullptr;
	}

	if ( m_PlayerScore > m_HighScore )
	{
		auto newState{ GetParent()->FindOrCreateState<GameNameEntryState>() };
		reinterpret_cast<GameNameEntryState*>( newState )->SetNewHighScore( m_PlayerScore );
		return newState;
	}

	return GetParent()->FindOrCreateState<GameStartState>();
}
void dae::GameScoreboardState::Enter()
{
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "challenging_stage_results.wav", 1.f );

	auto highScore{ functions::GetHighScore() };
	m_HighScore = highScore.first;
	for ( size_t idx{}; idx < 8; ++idx )
	{
		m_HighName[idx] = highScore.second[idx];
	}

	auto& levelScene{ SceneManager::GetInstance().GetScene( levelIdx ) };
	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	// Clear leftover enemies
	auto zakos{ gameScene.GetAllByTag( "zako"_hash ) };
	auto goeis{ gameScene.GetAllByTag( "goei"_hash ) };
	auto bosses{ gameScene.GetAllByTag( "boss"_hash ) };
	for ( auto& zako : zakos )
	{
		zako->MarkForRemoval();
	}
	for ( auto& goei : goeis )
	{
		goei->MarkForRemoval();
	}
	for ( auto& boss : bosses )
	{
		boss->MarkForRemoval();
	}
	//

	// Clear scoreboard
	uiScene.GetByTag( "scoreboard"_hash )->MarkForRemoval();
	//

	// Get transferred score
	auto transferredScore{ levelScene.GetByTag( "scoreTransferObject"_hash ) };
	m_PlayerScore = transferredScore->GetComponent<ScoreComponent>()->GetScore();
	transferredScore->MarkForRemoval();
	//

	// Scoreboard
	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto scoreText{ std::make_unique<GameObject>( "scoreboardObject"_hash ) };
	scoreText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( std::format( "Score - {}", m_PlayerScore ) );
	scoreText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f - 12.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( scoreText ) );

	if ( m_HighScore != 0 )
	{
		auto highNameText{ std::make_unique<GameObject>( "scoreboardObject"_hash ) };
		auto& textRef{
			highNameText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
				.SetIgnore( true )
		};
		if ( m_PlayerScore > m_HighScore )
		{
			textRef.SetText( std::format( "Previous High - {}", m_HighName.data() ) );
		}
		else
		{
			textRef.SetText( std::format( "High Score - {}", m_HighName.data() ) );
		}
		highNameText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f + 12.f },
															 TextAllignmentComponent::Allignment::center );
		uiScene.Add( std::move( highNameText ) );
		auto highScoreText{ std::make_unique<GameObject>( "scoreboardObject"_hash ) };
		highScoreText->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
			.SetIgnore( true )
			.SetText( std::format( "{}", m_HighScore ) );
		highScoreText->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f + 20.f },
															  TextAllignmentComponent::Allignment::center );
		uiScene.Add( std::move( highScoreText ) );
	}
	//

	m_StartPressed = false;

	const auto startKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::start ) };
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_RETURN, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		startKey, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );
}
void dae::GameScoreboardState::Exit()
{
	auto& levelScene{ dae::SceneManager::GetInstance().GetScene( levelIdx ) };
	auto& uiScene{ dae::SceneManager::GetInstance().GetScene( uiIdx ) };

	auto scoreTransferObjects{ levelScene.GetAllByTag( "scoreTransferObject"_hash ) };
	for ( auto& scoreTransferObject : scoreTransferObjects )
	{
		scoreTransferObject->MarkForRemoval();
	}

	auto scoreboardObjects{ uiScene.GetAllByTag( "scoreboardObject"_hash ) };
	for ( auto& scoreboardObject : scoreboardObjects )
	{
		scoreboardObject->MarkForRemoval();
	}
}

dae::GameNameEntryState::GameNameEntryState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::GameNameEntryState::Update()
{
	if ( m_StartPressed )
	{
		return GetParent()->FindOrCreateState<GameStartState>();
	}

	if ( !m_NameEntrySelector.Validate() || !m_NameEntryText.Validate() )
	{
		return nullptr;
	}

	m_NameEntryText->SetText( std::format( "{}", m_Name.data() ) );

	std::array selectorText{
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0',
	};
	selectorText[m_CurrentChar] = 'v';
	m_NameEntrySelector->SetText( selectorText.data() );

	return nullptr;
}
void dae::GameNameEntryState::Enter()
{
	m_CurrentChar = 0;
	m_Name = {
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0',
	};
	m_StartPressed = false;

	auto& uiScene{ SceneManager::GetInstance().GetScene( uiIdx ) };

	const std::string typefacePath{ "Typeface.png" };
	const std::string typefaceMapping{ "0123456789abcdefghijklmnopqrstuvwxyz-%.!" };

	auto nameEntryTitle{ std::make_unique<GameObject>( "nameEntryObject"_hash ) };
	nameEntryTitle->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "Enter Name" );
	nameEntryTitle->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f - 24.f },
														   TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( nameEntryTitle ) );

	auto nameEntrySelector{ std::make_unique<GameObject>( "nameEntryObject"_hash ) };
	nameEntrySelector->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true )
		.SetText( "v       " );
	m_NameEntrySelector = nameEntrySelector->GetComponent<PixelTextComponent>();
	nameEntrySelector->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f - 8.f },
															  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( nameEntrySelector ) );

	auto nameEntry{ std::make_unique<GameObject>( "nameEntryObject"_hash ) };
	nameEntry->AddComponent<dae::PixelTextComponent>( typefacePath, typefaceMapping, glm::vec2{ 8.f, 8.f } )
		.SetIgnore( true );
	m_NameEntryText = nameEntry->GetComponent<PixelTextComponent>();
	nameEntry->AddComponent<TextAllignmentComponent>( glm::vec2{ 288.f / 2.f, 224.f / 2.f },
													  TextAllignmentComponent::Allignment::center );
	uiScene.Add( std::move( nameEntry ) );

	Validator validator{ m_NameEntrySelector.GetControlBlock() };
	auto moveCursorLeft{ [&, validator]() mutable {
		if ( !validator.Validate() )
		{
			return;
		}

		--m_CurrentChar;
		if ( m_CurrentChar > 7 )
		{
			m_CurrentChar = 7;
		}
	} };
	auto moveCursorRight{ [&, validator]() mutable {
		if ( !validator.Validate() )
		{
			return;
		}

		++m_CurrentChar;
		if ( m_CurrentChar > 7 )
		{
			m_CurrentChar = 0;
		}
	} };
	auto incrementCurrentChar{ [&, validator]() mutable {
		if ( !validator.Validate() )
		{
			return;
		}

		auto currentChar{ m_Name[m_CurrentChar] };
		auto newChar{ currentChar + 1 };
		if ( currentChar == ' ' )
		{
			newChar = 'a';
		}
		if ( newChar == 'z' + 1 )
		{
			newChar = ' ';
		}
		m_Name[m_CurrentChar] = newChar;
	} };
	auto decrementCurrentChar{ [&, validator]() mutable {
		if ( !validator.Validate() )
		{
			return;
		}

		auto currentChar{ m_Name[m_CurrentChar] };
		auto newChar{ currentChar - 1 };
		if ( currentChar == 'a' )
		{
			newChar = ' ';
		}
		if ( newChar == ' ' - 1 )
		{
			newChar = 'z';
		}
		m_Name[m_CurrentChar] = newChar;
	} };
	auto backSpace{ [&, validator]() mutable {
		if ( !validator.Validate() )
		{
			return;
		}

		m_Name[m_CurrentChar] = ' ';
		if ( m_CurrentChar > 0 )
		{
			--m_CurrentChar;
		}
	} };

	const auto leftKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::left ) };
	const auto rightKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::right ) };
	const auto downKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::down ) };
	const auto upKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::up ) };
	const auto l1Key{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::l1 ) };
	const auto r1Key{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::r1 ) };
	const auto startKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::start ) };
	const auto eastKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::east ) };

	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_A, InputManager::KeyState::down, moveCursorLeft );
	InputManager::GetInstance().BindCommand<FunctionCommand>( leftKey, InputManager::KeyState::down, moveCursorLeft );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_D, InputManager::KeyState::down, moveCursorRight );
	InputManager::GetInstance().BindCommand<FunctionCommand>( rightKey, InputManager::KeyState::down, moveCursorRight );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_S, InputManager::KeyState::down, incrementCurrentChar );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		downKey, InputManager::KeyState::down, incrementCurrentChar );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		r1Key, InputManager::KeyState::held, incrementCurrentChar );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_W, InputManager::KeyState::down, decrementCurrentChar );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		upKey, InputManager::KeyState::down, decrementCurrentChar );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		l1Key, InputManager::KeyState::held, decrementCurrentChar );

	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_RETURN, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );
	InputManager::GetInstance().BindCommand<FunctionCommand>(
		startKey, InputManager::KeyState::down, [&]() mutable { m_StartPressed = true; } );

	InputManager::GetInstance().BindCommand<FunctionCommand>(
		SDL_SCANCODE_BACKSPACE, InputManager::KeyState::down, backSpace );
	InputManager::GetInstance().BindCommand<FunctionCommand>( eastKey, InputManager::KeyState::down, backSpace );
}
void dae::GameNameEntryState::Exit()
{
	std::array<char, 8> nameData{};
	for ( size_t idx{}; idx < nameData.size(); ++idx )
	{
		nameData[idx] = m_Name[idx];
	}
	functions::SetHighScore( m_Score, nameData );

	auto& uiScene{ SceneManager::GetInstance().GetScene( uiIdx ) };
	auto nameEntryObjects{ uiScene.GetAllByTag( "nameEntryObject"_hash ) };
	for ( auto& object : nameEntryObjects )
	{
		object->MarkForRemoval();
	}
}
void dae::GameNameEntryState::SetNewHighScore( uint32_t score )
{
	m_Score = score;
}
