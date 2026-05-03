#include "SceneManager.h"
#include <cassert>
#include "Scene.h"

dae::Scene& dae::SceneManager::CreateScene()
{
	m_Scenes.emplace_back( std::make_unique<std::pair<Scene, bool>>() );
	m_Scenes.back()->second = true;
	return m_Scenes.back()->first;
}

void dae::SceneManager::Destroy()
{
	m_Scenes.clear();
}

void dae::SceneManager::Update()
{
	for ( auto& scene : m_Scenes )
	{
		if ( !scene->second )
		{
			return;
		}
		scene->first.Update();
	}
}

void dae::SceneManager::Render() const
{
	for ( const auto& scene : m_Scenes )
	{
		if ( !scene->second )
		{
			return;
		}
		scene->first.Render();
	}
}

void dae::SceneManager::CleanUpRemovableObjects()
{
	for ( auto& scene : m_Scenes )
	{
		if ( !scene->second )
		{
			return;
		}
		scene->first.CleanUpRemovableObjects();
	}
}

dae::Scene& dae::SceneManager::GetScene( size_t idx ) const
{
	assert( idx < m_Scenes.size() && "Index is out of bounds!" );
	return m_Scenes[idx]->first;
}

void dae::SceneManager::EnableScene( size_t idx )
{
	assert( idx < m_Scenes.size() && "Index is out of bounds!" );
	m_Scenes[idx]->second = true;
}

void dae::SceneManager::DisableScene( size_t idx )
{
	assert( idx < m_Scenes.size() && "Index is out of bounds!" );
	m_Scenes[idx]->second = false;
}
