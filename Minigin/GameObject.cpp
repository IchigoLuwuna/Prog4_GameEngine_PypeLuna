#include <algorithm>
#include "GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/RenderComponent.h"

dae::GameObject::GameObject()
{
	AddComponent<TransformComponent>(); // All GameObjects require a transform
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

	auto child{ std::find( m_Children.begin(), m_Children.end(), pChild ) };
	( *child )->m_pParent = nullptr;
	m_Children.erase( child );
}
