#include "Enemy.h"
#include "Game/Context.h"

#include <Components.h>
#include <Core.h>
#include <Scene.h>

#include "Game/Components/AnimationComponent.h"
#include "Game/Components/DeathCallbackComponent.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/ObserverComponent.h"
#include "Game/Components/ReactiveSoundComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Game/Components/StateComponent.h"
#include "Game/Components/BrainComponent.h"
#include "Game/States/BossStates.h"
#include "Game/States/GoeiStates.h"
#include "Game/States/ZakoStates.h"
#include "Sound/SoundService.h"

std::unique_ptr<dae::GameObject> dae::functions::enemy::MakeZako()
{
	auto zako{ std::make_unique<dae::GameObject>( "zako"_hash ) };
#ifndef NDEBUG
	zako->AddComponent<dae::DebugComponent>( "zako" );
#endif
	zako->AddComponent<dae::BrainComponent>( HiveMind::HiveMindType::zako );
	zako->AddComponent<dae::SpriteSheetComponent>( "Enemy.png", dae::SpriteSheet::SpriteSheetInfo{ 24, 3 } );
	zako->AddComponent<dae::AnimationComponent>()
		.AddAnimation( "anim_Idle"_hash, { 6, 7, 0.5f, dae::AnimationComponent::LoopingMode::repeat } )
		.SetAnimation( "anim_Idle"_hash );
	zako->AddComponent<dae::HealthComponent>( 1 );
	zako->AddComponent<dae::StateComponent>().SetState<dae::ZakoReturningState>();
	zako->AddComponent<dae::ReactiveSoundComponent>().AddSound(
		{ "e_EntityDied"_hash, zako.get(), "zako_destroy.wav" } );
	zako->AddComponent<dae::HitboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f },
		std::vector{ "target_Player"_hash },
		[]( dae::GameObject* pParent, dae::Hurtbox* ) { pParent->GetComponent<dae::HealthComponent>()->Damage( 1 ); } );
	zako->AddComponent<dae::HurtboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f }, "target_Enemy"_hash, []( dae::GameObject* pParent, dae::Hitbox* ) {
			pParent->GetComponent<dae::HealthComponent>()->Damage( 1 );
		} );
	auto zakoPosRef{ zako->GetComponent<dae::TransformComponent>() };
	zako->AddComponent<dae::ObserverComponent>().AddCallback( "e_EntityDied"_hash, [=]( void* ) mutable {
		if ( !zakoPosRef.Validate() )
		{
			assert( false && "Failed to validate ship position" );
			return;
		}

		auto explosion{ std::make_unique<dae::GameObject>() };
		explosion->GetComponent<dae::TransformComponent>()->MoveTo( zakoPosRef->GetPosition() - glm::vec2{ 8.f, 8.f } );
		explosion->AddComponent<dae::SpriteSheetComponent>( "Effect32x32.png",
															dae::SpriteSheet::SpriteSheetInfo{ 5, 2 } );
		explosion->AddComponent<dae::AnimationComponent>()
			.AddAnimation( "anim_EnemyExplosion"_hash,
						   { 5, 8, 0.0666f, dae::AnimationComponent::LoopingMode::singleAndTerminate } )
			.SetAnimation( "anim_EnemyExplosion"_hash );

		dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( explosion ) );
	} );
	auto zakoObserverRef{ zako->GetComponent<dae::ObserverComponent>() };
	zako->GetComponent<dae::HealthComponent>()->RegisterObserver( zakoObserverRef );
	zako->AddComponent<dae::DeathCallbackComponent>(
		[=]() { dae::Minigin::eventManager.SendEvent( { "e_InsectDied"_hash, nullptr } ); } );

	return zako;
}

std::unique_ptr<dae::GameObject> dae::functions::enemy::MakeGoei()
{
	auto goei{ std::make_unique<dae::GameObject>( "goei"_hash ) };
#ifndef NDEBUG
	goei->AddComponent<dae::DebugComponent>( "goei" );
#endif
	goei->AddComponent<dae::BrainComponent>( HiveMind::HiveMindType::goei );
	goei->AddComponent<dae::SpriteSheetComponent>( "Enemy.png", dae::SpriteSheet::SpriteSheetInfo{ 24, 3 } );
	goei->AddComponent<dae::AnimationComponent>()
		.AddAnimation( "anim_Idle"_hash, { 30, 31, 0.5f, dae::AnimationComponent::LoopingMode::repeat } )
		.SetAnimation( "anim_Idle"_hash );
	goei->AddComponent<dae::HealthComponent>( 1 );
	goei->AddComponent<dae::StateComponent>().SetState<dae::GoeiReturningState>();
	goei->AddComponent<dae::ReactiveSoundComponent>().AddSound(
		{ "e_EntityDied"_hash, goei.get(), "goei_destroy.wav" } );
	goei->AddComponent<dae::HitboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f },
		std::vector{ "target_Player"_hash },
		[]( dae::GameObject* pParent, dae::Hurtbox* ) { pParent->GetComponent<dae::HealthComponent>()->Damage( 1 ); } );
	goei->AddComponent<dae::HurtboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f }, "target_Enemy"_hash, []( dae::GameObject* pParent, dae::Hitbox* ) {
			pParent->GetComponent<dae::HealthComponent>()->Damage( 1 );
		} );
	auto zakoPosRef{ goei->GetComponent<dae::TransformComponent>() };
	goei->AddComponent<dae::ObserverComponent>().AddCallback( "e_EntityDied"_hash, [=]( void* ) mutable {
		if ( !zakoPosRef.Validate() )
		{
			assert( false && "Failed to validate ship position" );
			return;
		}

		auto explosion{ std::make_unique<dae::GameObject>() };
		explosion->GetComponent<dae::TransformComponent>()->MoveTo( zakoPosRef->GetPosition() - glm::vec2{ 8.f, 8.f } );
		explosion->AddComponent<dae::SpriteSheetComponent>( "Effect32x32.png",
															dae::SpriteSheet::SpriteSheetInfo{ 5, 2 } );
		explosion->AddComponent<dae::AnimationComponent>()
			.AddAnimation( "anim_EnemyExplosion"_hash,
						   { 5, 8, 0.0666f, dae::AnimationComponent::LoopingMode::singleAndTerminate } )
			.SetAnimation( "anim_EnemyExplosion"_hash );

		dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( explosion ) );
	} );
	auto zakoObserverRef{ goei->GetComponent<dae::ObserverComponent>() };
	goei->GetComponent<dae::HealthComponent>()->RegisterObserver( zakoObserverRef );
	goei->AddComponent<dae::DeathCallbackComponent>(
		[=]() { dae::Minigin::eventManager.SendEvent( { "e_InsectDied"_hash, nullptr } ); } );

	return goei;
}

std::unique_ptr<dae::GameObject> dae::functions::enemy::MakeBoss()
{
	auto boss{ std::make_unique<dae::GameObject>( "boss"_hash ) };
#ifndef NDEBUG
	boss->AddComponent<dae::DebugComponent>( "boss" );
#endif
	boss->AddComponent<dae::BrainComponent>( HiveMind::HiveMindType::boss );
	boss->AddComponent<dae::SpriteSheetComponent>( "Enemy.png", dae::SpriteSheet::SpriteSheetInfo{ 24, 3 } );
	boss->AddComponent<dae::AnimationComponent>()
		.AddAnimation( "anim_Idle_Healthy"_hash, { 54, 55, 0.5f, dae::AnimationComponent::LoopingMode::repeat } )
		.AddAnimation( "anim_Idle_Damaged"_hash, { 70, 71, 0.5f, dae::AnimationComponent::LoopingMode::repeat } )
		.SetAnimation( "anim_Idle_Healthy"_hash );
	boss->AddComponent<dae::HealthComponent>( 2 );
	boss->AddComponent<dae::StateComponent>().SetState<dae::BossReturningState>();
	boss->AddComponent<dae::ReactiveSoundComponent>().AddSound(
		{ "e_EntityDied"_hash, boss.get(), "boss_destroy.wav" } );
	boss->AddComponent<dae::HitboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f },
		std::vector{ "target_Player"_hash },
		[]( dae::GameObject* pParent, dae::Hurtbox* ) { pParent->GetComponent<dae::HealthComponent>()->Damage( 1 ); } );
	boss->AddComponent<dae::HurtboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f }, "target_Enemy"_hash, []( dae::GameObject* pParent, dae::Hitbox* ) {
			pParent->GetComponent<dae::HealthComponent>()->Damage( 1 );
		} );
	auto bossPosRef{ boss->GetComponent<dae::TransformComponent>() };
	auto bossAnimRef{ boss->GetComponent<dae::AnimationComponent>() };
	boss->AddComponent<dae::ObserverComponent>()
		.AddCallback(
			"e_EntityDied"_hash,
			[=]( void* ) mutable {
				if ( !bossPosRef.Validate() )
				{
					assert( false && "Failed to validate ship position" );
					return;
				}

				auto explosion{ std::make_unique<dae::GameObject>() };
				explosion->GetComponent<dae::TransformComponent>()->MoveTo( bossPosRef->GetPosition() -
																			glm::vec2{ 8.f, 8.f } );
				explosion->AddComponent<dae::SpriteSheetComponent>( "Effect32x32.png",
																	dae::SpriteSheet::SpriteSheetInfo{ 5, 2 } );
				explosion->AddComponent<dae::AnimationComponent>()
					.AddAnimation( "anim_EnemyExplosion"_hash,
								   { 5, 8, 0.0666f, dae::AnimationComponent::LoopingMode::singleAndTerminate } )
					.SetAnimation( "anim_EnemyExplosion"_hash );

				dae::SceneManager::GetInstance().GetScene( gameIdx ).Add( std::move( explosion ) );
			} )
		.AddCallback( "e_HealthChanged"_hash, [=]( void* health ) mutable {
			if ( !bossAnimRef.Validate() )
			{
				assert( false && "Failed to validate ship animation component" );
				return;
			}

			auto pHealth{ reinterpret_cast<dae::HealthComponent*>( health ) };
			if ( pHealth->GetHealth() == 1 )
			{
				dae::ServiceLocator<dae::SoundService>::GetInstance().GetService().Play( "boss_damage.wav", 1.f );
				bossAnimRef->SetAnimation( "anim_Idle_Damaged"_hash );
			}
		} );
	auto bossObserverRef{ boss->GetComponent<dae::ObserverComponent>() };
	boss->GetComponent<dae::HealthComponent>()->RegisterObserver( bossObserverRef );

	return boss;
}
