#include "GameObject.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

dae::GameObject::~GameObject() = default;

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
