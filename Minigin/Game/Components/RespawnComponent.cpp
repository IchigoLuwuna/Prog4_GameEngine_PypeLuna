#include "RespawnComponent.h"
#include "Game/Context.h"
#include <Helpers.h>
#include <Scene.h>
#include <Components.h>

dae::RespawnComponent::RespawnComponent( GameObject* pParent,
										 std::function<std::unique_ptr<GameObject>()> respawnFunction,
										 glm::vec2 respawnPoint,
										 float timer )
	: Component( pParent )
	, m_RespawnFunction( respawnFunction )
	, m_RespawnPoint( respawnPoint )
	, m_RespawnTimer( timer )
{
}

void dae::RespawnComponent::Update()
{
	m_RespawnTimer -= Timer::GetInstance().GetElapsed();

	if ( m_RespawnTimer > 0.f )
	{
		return;
	}

	auto spawnedObject{ m_RespawnFunction() };
	spawnedObject->GetComponent<dae::TransformComponent>()->MoveTo( m_RespawnPoint );
	auto& gameScene{ SceneManager::GetInstance().GetScene( gameIdx ) };
	gameScene.Add( std::move( spawnedObject ) );
	GetParent()->MarkForRemoval();
}
