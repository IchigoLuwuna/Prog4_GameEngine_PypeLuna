#include "Enemy.h"
#include <Components.h>
#include <Core.h>
#include "Game/Components/AnimationComponent.h"
#include "Game/Components/DeathCallbackComponent.h"
#include "Game/Components/HealthComponent.h"
#include "Game/Components/ReactiveSoundComponent.h"
#include "Game/Components/SpriteSheetComponent.h"
#include "Game/Components/StateComponent.h"
#include "Game/States/ZakoStates.h"

std::unique_ptr<dae::GameObject> dae::functions::enemy::MakeZako()
{
	auto zako{ std::make_unique<dae::GameObject>() };
	zako->AddComponent<dae::SpriteSheetComponent>( "Enemy.png", dae::SpriteSheet::SpriteSheetInfo{ 24, 3 } );
	zako->AddComponent<dae::AnimationComponent>()
		.AddAnimation( "anim_Idle"_hash, { 6, 7, 0.5f, dae::AnimationComponent::LoopingMode::repeat } )
		.SetAnimation( "anim_Idle"_hash );
	zako->AddComponent<dae::HealthComponent>( 1 );
	zako->AddComponent<dae::StateComponent<dae::ZakoState>>().SetState<dae::ZakoReturningState>();
	zako->AddComponent<dae::ReactiveSoundComponent>().AddSound(
		{ "e_EntityDied"_hash, zako.get(), "zako_destroy.wav" } );
	zako->AddComponent<dae::HitboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f }, std::vector{ "target_Player"_hash }, nullptr );
	zako->AddComponent<dae::HurtboxComponent>(
		glm::vec4{ 2.f, 3.f, 13.f, 10.f }, "target_Enemy"_hash, []( dae::GameObject* pParent, dae::Hitbox* ) {
			pParent->GetComponent<dae::HealthComponent>()->Damage( 1 );
		} );
	zako->AddComponent<dae::DeathCallbackComponent>(
		[=]() { dae::Minigin::eventManager.SendEvent( { "e_InsectDied"_hash, nullptr } ); } );

	return zako;
}
