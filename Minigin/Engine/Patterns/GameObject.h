#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <vector>
#include "Component.h"
#include "Engine/Memory/SafePtr.h"
#include "Engine/Memory/ReferencePtr.h"

namespace dae
{
class Texture2D;
class Component;
class GameObject final
{
public:
	GameObject();
	~GameObject();
	GameObject( const GameObject& other ) = delete;
	GameObject( GameObject&& other ) = default;
	GameObject& operator=( const GameObject& other ) = delete;
	GameObject& operator=( GameObject&& other ) = default;

	void Update();
	void Render() const;

	int GetChildCount() const;
	GameObject* GetChildAt( int index );
	void SetParent( GameObject* pParent );
	GameObject* GetParent()
	{
		return m_pParent;
	}

	template <typename ComponentType, typename... Args>
		requires std::derived_from<ComponentType, Component> &&
				 requires( Args... args ) { ComponentType( nullptr, args... ); }
	void AddComponent( const Args&... args )
	{
		SafePtr<ComponentType> component{ this, args... };
		m_Components.push_back( SafePtr<Component>( std::move( component ) ) );
	}
	template <typename ComponentType>
		requires std::derived_from<ComponentType, Component>
	void AddComponent( std::unique_ptr<ComponentType>&& pComponent )
	{
		m_Components.push_back( std::move( pComponent ) );
	}

	template <typename T>
	ReferencePtr<T> GetComponent() const
	{
		for ( auto& component : m_Components )
		{
			T* pComponent{ dynamic_cast<T*>( component.Get() ) };
			if ( pComponent )
			{
				return ReferencePtr<T>( component );
			}
		}

		return ReferencePtr<T>();
	}

	void MarkForRemoval();
	bool IsMarkedForRemoval() const;

private:
	GameObject* m_pParent{};
	std::vector<GameObject*> m_Children{};

	std::vector<SafePtr<Component>> m_Components{};

	bool m_MarkedForRemoval{};

	void AddChild( GameObject* pChild );
	void RemoveChild( GameObject* pChild );
};
} // namespace dae
#endif
