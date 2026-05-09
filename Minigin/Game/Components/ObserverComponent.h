#ifndef OBSERVERCOMPONENT_H
#define OBSERVERCOMPONENT_H
#include <functional>
#include <unordered_map>
#include <Patterns.h>

namespace dae
{
class ObserverComponent : public Component, public Observer
{
public:
	ObserverComponent( GameObject* pParent );
	virtual ~ObserverComponent() = default;

	virtual void Update() override
	{
	}

	ObserverComponent& AddCallback( size_t eventHash, std::function<void( void* )> callback );
	ObserverComponent& RemoveCallback( size_t eventHash );

	virtual void Notify( size_t eventHash, void* pSubject ) override;

private:
	std::unordered_map<size_t, std::function<void( void* )>> m_ObserverCallbacks{};
};
} // namespace dae
#endif
