#include <algorithm>
#include <cassert>
#include "Patterns/GameObject.h"
#include "Scene.h"

void dae::Scene::Add( std::unique_ptr<GameObject> object )
{
	assert( object && "Cannot add a null GameObject to the scene." );
	m_RequestedObjects.push_back( std::move( object ) );
}

void dae::Scene::AddRequested()
{
	for ( auto& requested : m_RequestedObjects )
	{
		m_Objects.push_back( std::move( requested ) );
	}

	m_RequestedObjects.clear();
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

dae::GameObject* dae::Scene::GetByTag( size_t tag ) const
{
	for ( auto& object : m_RequestedObjects )
	{
		if ( object->GetTag() == tag )
		{
			return object.get();
		}
	}
	for ( auto& object : m_Objects )
	{
		if ( object->GetTag() == tag )
		{
			return object.get();
		}
	}

	return nullptr;
}

std::vector<dae::GameObject*> dae::Scene::GetAllByTag( size_t tag ) const
{
	std::vector<GameObject*> objects( m_Objects.size() );

	for ( auto& object : m_RequestedObjects )
	{
		if ( object->GetTag() == tag )
		{
			objects.push_back( object.get() );
		}
	}
	for ( auto& object : m_Objects )
	{
		if ( object->GetTag() == tag )
		{
			objects.push_back( object.get() );
		}
	}

	return objects;
}

dae::GameObject* dae::Scene::GetByTags( size_t* begin, size_t* end )
{
	assert( begin <= end && "Begin must be smaller than end" );

	for ( auto& object : m_RequestedObjects )
	{
		for ( ; begin != end; ++begin )
		{
			if ( object->GetTag() == *begin )
			{
				return object.get();
			}
		}
	}
	for ( auto& object : m_Objects )
	{
		for ( ; begin != end; ++begin )
		{
			if ( object->GetTag() == *begin )
			{
				return object.get();
			}
		}
	}

	return nullptr;
}

void dae::Scene::CleanUpRemovableObjects()
{
	m_Objects.erase( std::remove_if( m_Objects.begin(),
									 m_Objects.end(),
									 []( auto& object ) { return object->IsMarkedForRemoval(); } ),
					 m_Objects.end() );
}
