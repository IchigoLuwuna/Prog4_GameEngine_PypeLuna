#include "SceneManager.h"
#include "Scene.h"

dae::Scene& dae::SceneManager::CreateScene()
{
	m_Scenes.emplace_back( std::make_unique<Scene>() );
	return *m_Scenes.back();
}

void dae::SceneManager::Destroy()
{
	m_Scenes.clear();
}

void dae::SceneManager::Update()
{
	for ( auto& scene : m_Scenes )
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for ( const auto& scene : m_Scenes )
	{
		scene->Render();
	}
}

void dae::SceneManager::CleanUpRemovableObjects()
{
	for ( auto& scene : m_Scenes )
	{
		scene->CleanUpRemovableObjects();
	}
}
