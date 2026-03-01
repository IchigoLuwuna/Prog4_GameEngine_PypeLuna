#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <memory>
#include <vector>
#include "Component.h"

namespace dae
{
class Texture2D;
class GameObject final
{
public:
	virtual void Update();
	virtual void Render() const;

	template <typename T, typename... Types>
		requires std::derived_from<T, Component>
	void AddComponent( const Types&... args )
	{
		m_Components.push_back( std::make_unique<T>( this, args... ) );
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

	GameObject() = default;
	~GameObject();
	GameObject( const GameObject& other ) = delete;
	GameObject( GameObject&& other ) = default;
	GameObject& operator=( const GameObject& other ) = delete;
	GameObject& operator=( GameObject&& other ) = default;

private:
	std::vector<std::unique_ptr<Component>> m_Components{};
};
} // namespace dae
#endif
