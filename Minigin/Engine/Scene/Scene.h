#pragma once
#include <memory>
#include <vector>
#include "Patterns/GameObject.h"

namespace dae
{
class Scene final
{
public:
	Scene() = default;

	void Add( std::unique_ptr<GameObject> object );
	void AddRequested();
	void Remove( const GameObject& object );
	void RemoveAll();

	void Update();
	void Render() const;

	GameObject* GetByTag( size_t tag ) const;

	void CleanUpRemovableObjects();

	Scene( const Scene& other ) = delete;
	Scene( Scene&& other ) = delete;
	Scene& operator=( const Scene& other ) = delete;
	Scene& operator=( Scene&& other ) = delete;

private:
	std::vector<std::unique_ptr<GameObject>> m_Objects{};
	std::vector<std::unique_ptr<GameObject>> m_RequestedObjects{};
};

} // namespace dae
