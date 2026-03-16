#include <algorithm>
#include <cassert>
#include "Scene.h"

void dae::Scene::Add( std::unique_ptr<GameObject> object )
{
	assert( object && "Cannot add a null GameObject to the scene." );
	m_Objects.emplace_back( std::move( object ) );
}

void dae::Scene::Remove( const GameObject& object )
{
	m_Objects.erase( std::remove_if( m_Objects.begin(),
									 m_Objects.end(),
									 [&object]( const auto& ptr ) { return ptr.get() == &object; } ),
					 m_Objects.end() );
}

void dae::Scene::RemoveAll()
{
	m_Objects.clear();
}

void dae::Scene::Update()
{
	for ( auto& object : m_Objects )
	{
		object->Update();
	}
}

void dae::Scene::Render() const
{
	for ( const auto& object : m_Objects )
	{
		object->Render();
	}
}

void dae::Scene::CleanUpRemovableObjects()
{
	m_Objects.erase( std::remove_if( m_Objects.begin(),
									 m_Objects.end(),
									 []( auto& object ) { return object->IsMarkedForRemoval(); } ),
					 m_Objects.end() );
}
