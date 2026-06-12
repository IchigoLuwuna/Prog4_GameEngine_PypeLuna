#include "Player.h"
#include <Core.h>
#include "Game/Components/GamepadControlComponent.h"
#include "Game/Components/LivesComponent.h"
#include "Game/Components/RespawnComponent.h"
#include "Game/Context.h"

#include <Components.h>
#include <Scene.h>
#include <Sound.h>
#include <Commands.h>
#include <Input.h>

#include "Game/Components/AnimationComponent.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/ObserverComponent.h"
#include "Game/Components/ProjectileAmmoComponent.h"
#include "Game/Components/ProjectileComponent.h"
#include "Game/Components/ReactiveSoundComponent.h"
#include "Game/Components/ScoreComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Game/Components/ScoreDisplayComponent.h"

#include "Game/Commands/DamageCommand.h"

std::unique_ptr<dae::GameObject> dae::functions::player::MakePlayer()
{
	auto ship{ std::make_unique<GameObject>( "player"_hash ) };

	ship->AddComponent<dae::SpriteSheetComponent>( "Ship.png", dae::SpriteSheet::SpriteSheetInfo{ 8, 3 } )
		.SetIndex( 6, 0 );

	ship->AddComponent<dae::HealthComponent>( 1, 1, 3.f );
	auto shipHealthRef{ ship->GetComponent<dae::HealthComponent>() };
	ship->AddComponent<dae::LivesComponent>( 1 );
	auto shipLivesRef{ ship->GetComponent<dae::LivesComponent>() };

	std::vector<std::pair<size_t, uint32_t>> shipScoreGainOnEvent{ { "e_InsectDied"_hash, 100 } };
	ship->AddComponent<dae::ScoreComponent>( std::move( shipScoreGainOnEvent ) );

	ship->AddComponent<dae::ProjectileAmmoComponent>( 2 );

	ship->AddComponent<dae::ReactiveSoundComponent>().AddSound( { "e_EntityDied"_hash, ship.get(), "explosion.wav" } );

	ship->AddComponent<dae::HurtboxComponent>(
		glm::vec4{ 0.f, 0.f, 16.f, 15.f }, "target_Player"_hash, []( dae::GameObject* pParent, auto ) {
			pParent->GetComponent<dae::HealthComponent>()->Damage( 3 );
		} );

	auto shipPosRef{ ship->GetComponent<dae::TransformComponent>() };
	auto scoreRef{ ship->GetComponent<dae::ScoreComponent>() };
	ship->AddComponent<dae::ObserverComponent>().AddCallback( "e_EntityDied"_hash, [=]( void* ) mutable {
		if ( !shipPosRef.Validate() || !shipLivesRef.Validate() || !scoreRef.Validate() )
		{
			assert( false && "Failed to validate ship" );
			return;
		}

		// Play Explosion Anim
		auto explosion{ std::make_unique<dae::GameObject>() };
		explosion->GetComponent<dae::TransformComponent>()->MoveTo( shipPosRef->GetPosition() - glm::vec2{ 8.f, 8.f } );
		explosion->AddComponent<dae::SpriteSheetComponent>( "Effect32x32.png",
															dae::SpriteSheet::SpriteSheetInfo{ 5, 2 } );
		explosion->AddComponent<dae::AnimationComponent>()
			.AddAnimation( "anim_ShipExplosion"_hash,
						   { 0, 3, 0.15f, dae::AnimationComponent::LoopingMode::singleAndTerminate } )
			.SetAnimation( "anim_ShipExplosion"_hash );
		dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( explosion ) );
		//

		auto& levelScene{ SceneManager::GetInstance().GetScene( levelIdx ) };
		if ( shipLivesRef->GetLives() == 0 )
		{
			dae::Minigin::eventManager.SendEvent( { "e_ShipRanOutOfLives"_hash } );
			auto scoreTransferObject{ std::make_unique<GameObject>( "scoreTransferObject"_hash ) };
			scoreTransferObject->AddComponent<ScoreComponent>( std::vector<std::pair<size_t, uint32_t>>{},
															   scoreRef->GetScore() );
			levelScene.Add( std::move( scoreTransferObject ) );
			return;
		}

		// Set Respawner
		auto transferredScore{ scoreRef->GetScore() };
		auto transferredLives{ shipLivesRef->GetLives() - 1 };
		auto respawn{ [=]() {
			auto object{ MakePlayer() };
			object->GetComponent<ScoreComponent>()->Accumulate( transferredScore );
			object->GetComponent<LivesComponent>()->SetLives( transferredLives );
			return object;
		} };
		auto respawner{ std::make_unique<dae::GameObject>( "respawner"_hash ) };
		respawner->AddComponent<dae::RespawnComponent>( respawn, glm::vec2{ 288.f / 2.f, 192.f }, 4.f );
		levelScene.Add( std::move( respawner ) );
		//
	} );

#ifndef NDEBUG
	ship->AddComponent<dae::DebugComponent>( "ship" );
#endif

	auto shipObserverRef{ ship->GetComponent<dae::ObserverComponent>() };
	shipHealthRef->RegisterObserver( shipObserverRef );

	BindInputForPlayer( ship.get() );
	BindScoreboardForPlayer( ship.get() );

	return ship;
}

void dae::functions::player::BindInputForPlayer( GameObject* pPlayer )
{
	pPlayer->AddComponent<dae::GamepadControlComponent>( 96.f );

	auto shipPosRef{ pPlayer->GetComponent<dae::TransformComponent>() };
	auto shipAmmoRef{ pPlayer->GetComponent<dae::ProjectileAmmoComponent>() };

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
		projectile->AddComponent<dae::HitboxComponent>(
			glm::vec4{ 2.f, 0.f, 3.f, 8.f }, std::vector{ "target_Enemy"_hash }, []( dae::GameObject* pParent, auto ) {
				pParent->MarkForRemoval();
			} );

		dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( projectile ) );
	} };

	// Bindings
	const auto leftKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::left ) };
	const auto rightKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::right ) };
	const auto southKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::south ) };
	const auto eastKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::east ) };
	const auto westKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::west ) };
	const auto northKey{ dae::Gamepad::RemapButtonToKey( dae::Gamepad::Button::north ) };

	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_A, dae::InputManager::KeyState::held, moveLeft );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		leftKey, dae::InputManager::KeyState::held, moveLeft );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_D, dae::InputManager::KeyState::held, moveRight );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		rightKey, dae::InputManager::KeyState::held, moveRight );

	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_J, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		SDL_SCANCODE_K, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		southKey, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		eastKey, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		westKey, dae::InputManager::KeyState::down, shoot );
	dae::InputManager::GetInstance().BindCommand<dae::FunctionCommand>(
		northKey, dae::InputManager::KeyState::down, shoot );
}

void dae::functions::player::BindScoreboardForPlayer( GameObject* pPlayer )
{
	auto* pScoreboard{ SceneManager::GetInstance().GetScene( uiIdx ).GetByTag( "scoreboard"_hash ) };
	if ( !pScoreboard )
	{
		return;
	}

	pPlayer->GetComponent<dae::ScoreComponent>()->RegisterObserver(
		pScoreboard->GetComponent<dae::ScoreDisplayComponent>() );
}
