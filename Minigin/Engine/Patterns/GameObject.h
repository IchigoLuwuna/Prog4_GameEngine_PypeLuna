#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <vector>
#include "Component.h"

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
		m_Components.push_back( std::make_unique<ComponentType>( this, args... ) );
	}

	template <typename T>
	T* GetComponent()
	{
		for ( auto& component : m_Components )
		{
			T* pComponent{ dynamic_cast<T*>( component.get() ) };
			if ( pComponent )
			{
				return pComponent;
			}
		}

		return nullptr;
	}

	void MarkForRemoval();
	bool IsMarkedForRemoval() const;

private:
	GameObject* m_pParent{};
	std::vector<GameObject*> m_Children{};

	std::vector<std::unique_ptr<Component>> m_Components{};

	bool m_MarkedForRemoval{};

	void AddChild( GameObject* pChild );
	void RemoveChild( GameObject* pChild );
};
} // namespace dae
#endif
