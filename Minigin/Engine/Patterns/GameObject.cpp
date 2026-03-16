#include "GameObject.h"
#include <algorithm>
#include "Engine/Components/TransformComponent.h"
#include "RenderComponent.h"

dae::GameObject::GameObject()
{
	AddComponent<TransformComponent>(); // All GameObjects require a transform
}

dae::GameObject::~GameObject()
{
	if ( m_pParent )
	{
		m_pParent->RemoveChild( this );
	}
}

void dae::GameObject::Update()
{
	for ( auto& upComponent : m_Components )
	{
		upComponent->Update();
	}
}

void dae::GameObject::Render() const
{
	for ( auto& upComponent : m_Components )
	{
		auto pRenderComponent{ dynamic_cast<RenderComponent*>( upComponent.get() ) };
		if ( pRenderComponent )
		{
			pRenderComponent->Render();
		}
	}
}

int dae::GameObject::GetChildCount() const
{
	return static_cast<int>( m_Children.size() );
}

dae::GameObject* dae::GameObject::GetChildAt( int index )
{
	return m_Children[index];
}

void dae::GameObject::SetParent( GameObject* pParent )
{
	if ( !pParent || pParent == this )
	{
		return;
	}

	if ( m_pParent )
	{
		m_pParent->RemoveChild( this );
	}
	m_pParent = pParent;
	pParent->AddChild( this );
	GetComponent<dae::TransformComponent>()->MarkForUpdate();
}

void dae::GameObject::AddChild( GameObject* pChild )
{
	if ( !pChild || pChild == this )
	{
		return;
	}

	m_Children.push_back( pChild );
}

void dae::GameObject::RemoveChild( GameObject* pChild )
{
	if ( !pChild || pChild == this )
	{
		return;
	}

	for ( int index{}; index < static_cast<int>( m_Children.size() ); ++index )
	{
		auto child{ m_Children[index] };
		if ( child != pChild )
		{
			continue;
		}

		child->m_pParent = nullptr;
		child = nullptr;
		std::swap( m_Children[index], m_Children.back() );
		m_Children.pop_back();
		--index;
	}
}

void dae::GameObject::MarkForRemoval()
{
	m_MarkedForRemoval = true;
	for ( auto* child : m_Children ) // Recurse down scene graph
	{
		child->MarkForRemoval();
	}
}

bool dae::GameObject::IsMarkedForRemoval() const
{
	return m_MarkedForRemoval;
}
