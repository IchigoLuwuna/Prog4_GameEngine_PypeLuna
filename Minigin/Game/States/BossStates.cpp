#include "BossStates.h"
#include "Components/HitboxComponent.h"
#include "Components/TransformComponent.h"
#include <Random.h>
#include <Sound.h>
#include "Game/Components/AnimationComponent.h"
#include "Game/Components/TractorComponent.h"
#include "Game/Context.h"
#include "Scene/SceneManager.h"
#include "States/StateMachine.h"
#include <Helpers.h>

dae::BossIdlingState::BossIdlingState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::BossIdlingState::Update()
{
	m_StateTime -= Timer::GetInstance().GetElapsed();

	if ( m_StateTime > 0.f )
	{
		return nullptr;
	}

	if ( m_BrainRef->IsTractoring() )
	{
		m_StateTime = random::GetRand( 1.f, 1.f );
		return nullptr;
	}

	return GetParent()->FindOrCreateState<BossDivingState>();
}
void dae::BossIdlingState::Enter()
{
	m_StateTime = random::GetRand( 1.f, 1.f );

	// Get Brain reference
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}
}
void dae::BossIdlingState::Exit()
{
}

dae::BossDivingState::BossDivingState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::BossDivingState::Update()
{
	m_DivingTime += Timer::GetInstance().GetElapsed();
	constexpr float divingTimeMax{ 2.f };
	const float lerp{ m_DivingTime / divingTimeMax };

	if ( !m_PlayerFound )
	{
		return GetParent()->FindOrCreateState<BossReturningState>();
	}

	if ( m_DivingTime >= divingTimeMax )
	{
		return GetParent()->FindOrCreateState<BossTractorBeamState>();
	}

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };

	auto lerpedPos{ m_BezierPath.Lerp( lerp ) };
	transform->MoveTo( lerpedPos.x, lerpedPos.y );

	return nullptr;
}
void dae::BossDivingState::Enter()
{
	// Get Brain reference
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}

	// Verify there is a player to target
	auto pPlayer{ m_BrainRef->GetPlayer() };
	if ( !pPlayer )
	{
		m_PlayerFound = false;
		return;
	}
	m_PlayerFound = true;

	ServiceLocator<SoundService>::GetInstance().GetService().Play( "dive.wav", 1.f );
	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };

	// Set Bezier Path
	glm::vec2 a{ transform->GetPosition() };
	glm::vec2 b{ transform->GetPosition().x + 64.f, transform->GetPosition().y - 64.f };
	glm::vec2 c{ transform->GetPosition().x - 64.f, transform->GetPosition().y - 64.f };
	glm::vec2 d{ pPlayer->GetComponent<dae::TransformComponent>()->GetPosition().x, 224.f / 2.f };
	bool RandomDivingWindingFlip{ dae::random::GetRand( 50 ) };
	if ( RandomDivingWindingFlip )
	{
		std::swap( b, c );
	}
	m_BezierPath.controlPoints = { a, b, c, d };
	m_DivingTime = 0.f;

	m_BrainRef->SetTractoring();
}
void dae::BossDivingState::Exit()
{
}

dae::BossReturningState::BossReturningState( StateMachine* pParent )
	: State( pParent )
{
}

dae::State* dae::BossReturningState::Update()
{
	constexpr float speed{ 92.f };

	auto transform{ GetParent()->GetParent()->GetComponent<dae::TransformComponent>() };
	auto target{ m_BrainRef->GetFormationPosition() };
	auto movement{ glm::normalize( target - transform->GetPosition() ) * speed };
	movement *= Timer::GetInstance().GetElapsed();

	transform->Move( movement );

	float epsilon{ speed * Timer::GetInstance().GetElapsed() * 1.1f }; // Dynamic snapping factor
	// Not at target pos
	if ( transform->GetPosition().x > target.x + epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().x < target.x - epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().y > target.y + epsilon )
	{
		return nullptr;
	}
	if ( transform->GetPosition().y < target.y - epsilon )
	{
		return nullptr;
	}
	//

	// Snap to target and change state to idle
	transform->MoveTo( target );
	return GetParent()->FindOrCreateState<BossIdlingState>();
}
void dae::BossReturningState::Enter()
{
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}
}
void dae::BossReturningState::Exit()
{
}

dae::BossTractorBeamState::BossTractorBeamState( StateMachine* pParent )
	: State( pParent )
{
}
dae::State* dae::BossTractorBeamState::Update()
{
	m_StateTime -= Timer::GetInstance().GetElapsed();

	if ( m_StateTime <= 0.f )
	{
		return GetParent()->FindOrCreateState<BossReturningState>();
	}

	if ( m_TractorCaptured )
	{
		return GetParent()->FindOrCreateState<BossReturningState>();
	}

	return nullptr;
}
void dae::BossTractorBeamState::Enter()
{
	if ( !m_BrainRef.Validate() )
	{
		m_BrainRef = GetParent()->GetParent()->GetComponent<BrainComponent>();
		if ( !m_BrainRef.Validate() )
		{
			assert( false && "Could not find a brain" );
		}
	}

	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };

	m_TractorCaptured = false;

	m_StateTime = 5.01f;
	ServiceLocator<SoundService>::GetInstance().GetService().Play( "tractor_beam.wav", 1.f );

	auto tractor{ MakeTractorBeam() };
	auto bossPos{ GetParent()->GetParent()->GetComponent<TransformComponent>()->GetPosition() };
	tractor->GetComponent<TransformComponent>()->MoveTo( { bossPos + glm::vec2{ -16.f, 16.f } } );
	gameScene.Add( std::move( tractor ) );
}
void dae::BossTractorBeamState::Exit()
{
	m_BrainRef->ClearTractoring();

	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };
	auto pTractor{ gameScene.GetByTag( "tractor"_hash ) };
	if ( pTractor )
	{
		pTractor->MarkForRemoval();
	}
}
void dae::BossTractorBeamState::MarkTractorCaptured()
{
	m_TractorCaptured = true;
}
std::unique_ptr<dae::GameObject> dae::BossTractorBeamState::MakeTractorBeam()
{
	Validator bossValidator{ GetParent()->GetParent()->GetComponent<TransformComponent>().GetControlBlock() };
	BossTractorBeamState* pState{ this };

	auto tractor{ std::make_unique<GameObject>( "tractor"_hash ) };
	tractor->AddComponent<HitboxComponent>(
		glm::vec4{ 0.f, 0.f, 48.f, 0.f }, std::vector{ "target_Player"_hash }, [=]( GameObject*, Hurtbox* ) mutable {
			// Make sure we are not doing dangling pointer stuff
			if ( !bossValidator.Validate() )
			{
				return;
			}

			pState->MarkTractorCaptured();
		} );
	tractor->AddComponent<SpriteSheetComponent>( "Tractor.png", SpriteSheet::SpriteSheetInfo{ 3, 10 } );
	tractor->AddComponent<AnimationComponent>()
		.AddAnimation( "anim_Tractor"_hash, { 0, 29, 5.1f / 30.f } )
		.SetAnimation( "anim_Tractor"_hash );
	tractor->AddComponent<TractorComponent>();

	return tractor;
}
